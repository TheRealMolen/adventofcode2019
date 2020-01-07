#include <algorithm>
#include <iterator>
#include <map>
#include <queue>
#include <vector>

#include "harness.h"
#include "util.h"



typedef pt2d<int16_t> d18_pt;
static const vector<d18_pt> d18_dirs = { { 0,-1 },{ 1,0 },{ 0,1 },{ -1,0 } };

struct d18_map2d
{
    size_t width;
    size_t height;
    vector<char> tiles;

    d18_map2d(const stringlist& lines)
    {
        width = lines.front().size();
        height = lines.size();
        _ASSERT(width < 0x7fff && height < 0x7fff);

        tiles.reserve(width * height);

        for (const auto& line : lines)
            copy(line.begin(), line.end(), back_inserter(tiles));
    }

    d18_pt find_item(char item) const
    {
        auto it = find(tiles.begin(), tiles.end(), item);
        if (it == tiles.end())
            return d18_pt(-1, -1);

        auto index = distance(tiles.begin(), it);

        return d18_pt((int16_t)(index % width), (int16_t)(index / width));
    }

    char get(size_t x, size_t y) const
    {
        return tiles[x + y*width];
    }
    char get(const d18_pt& p) const
    {
        return tiles[p.x + p.y*width];
    }

    size_t calc_ix(const d18_pt& p) const
    {
        return p.x + p.y * width;
    }
    d18_pt pt_from_ix(size_t ix) const
    {
        return d18_pt((int16_t)(ix % width), (int16_t)(ix / width));
    }

    int try_path(const d18_pt& from, const d18_pt& to, uint32_t* pout_keys_needed) const;
};
ostream& operator<<(ostream& os, const d18_map2d& m)
{
    auto it = m.tiles.begin();
    for (size_t y = 0; y < m.height; ++y, it += m.width)
    {
        os.write(&*it, m.width);
        os << '\n';
    }
    return os;
}


int d18_map2d::try_path(const d18_pt& from, const d18_pt& to, uint32_t* pout_keys_needed = NULL) const
{
    // indexed same as tiles
    vector<bool> visited(tiles.size(), false);
    vector<uint16_t> distances(tiles.size(), 0xffffu);

    // start point has distance of zero
    distances[calc_ix(from)] = 0;

    vector<size_t> unvisited_by_ix;
    unvisited_by_ix.reserve(tiles.size());
    unvisited_by_ix.push_back(calc_ix(from));

    bool reached = false;
    while (!unvisited_by_ix.empty())
    {
        // find unvisited with the lowest distance
        auto it_curr_ix = min_element(unvisited_by_ix.begin(), unvisited_by_ix.end(),
            [&](auto ix_a, auto ix_b) { return distances[ix_a] < distances[ix_b]; });
        auto curr_ix = *it_curr_ix;
        unvisited_by_ix.erase(it_curr_ix);

        auto curr_p = pt_from_ix(curr_ix);
        if (curr_p == to)
        {
            reached = true;
            break;
        }

        // there may be dupes in the unvisited list
        if (visited[curr_ix])
            continue;
        visited[curr_ix] = true;

        auto curr_dist = distances[curr_ix];

        uint16_t new_ndist = curr_dist + 1;

        // spin through unvisited neighbours and update their distances if need be
        for (auto& dir : d18_dirs)
        {
            auto n = curr_p + dir;
            size_t nindex = calc_ix(n);
            if (visited[nindex])
                continue;

            auto ntile = tiles[nindex];
            if (ntile == '#')
                continue;

            if (new_ndist < distances[nindex])
            {
                distances[nindex] = new_ndist;
                unvisited_by_ix.push_back(nindex);
            }
        }
    }

    if (!reached)
        return -1;

    // follow the path to find any doors we encountered
    uint32_t doors = 0;
    auto curr = to;
    auto curr_d = distances[calc_ix(curr)];
    do {
        auto curr_ix = calc_ix(curr);

        auto tile = tiles[curr_ix];
        if (tile >= 'A' && tile <= 'Z')
        {
            doors |= (1 << (tile - 'A'));
        }

        // look for neighbour with next-lowest distance
        curr_d--;
        bool found = false;
        for (auto& dir : d18_dirs)
        {
            auto prev = curr + dir;
            auto prev_ix = calc_ix(prev);
            if (distances[prev_ix] == curr_d)
            {
                curr = prev;
                found = true;
                break;
            }
        }
        _ASSERT(found);
    } while (curr != from);

    if (pout_keys_needed)
        *pout_keys_needed = doors;
    return distances[calc_ix(to)];
}

struct d18_edge
{
    char ends[2];
    uint16_t cost;
    uint32_t keys_needed;   // bit0 is 'a', bit1 is 'b', ...
};

struct d18_blop
{
    char key;
    bool visited;
    const d18_blop* parent;
    uint32_t cost;
    uint32_t keys_owned;
    vector<pair<char, uint16_t>> possible;

    d18_blop(char k, const d18_blop* p, uint32_t c, uint32_t ko, const map<char, vector<const d18_edge*>>& nodes)
        : key(k), visited(false), parent(p), cost(c), keys_owned(ko)
    {
        for (const auto& e : nodes.at(key))
        {
            if (e->keys_needed & ~ko)
                continue;

            char n = e->ends[0] != k ? e->ends[0] : e->ends[1];

            // omit visited keys
            uint32_t nbit = 1 << (n - 'a');
            if (ko & nbit)
                continue;

            possible.push_back(make_pair(n, e->cost));
        }
    }
};

ostream& operator<<(ostream& os, const d18_blop& b)
{
    os << " .. from " << b.key << ": [ ";
    for (auto& next : b.possible)
    {
        if (next != b.possible.front())
            os << ", ";
        os << next.first << " (" << next.second << ")";
    }
    os << " ]";
    return os;
}

struct d18_blop_prio_cmp
{
    bool operator()(const d18_blop* lhs, const d18_blop* rhs) const
    {
        return lhs->cost > rhs->cost;
    }
    bool operator()(const d18_blop& lhs, const d18_blop& rhs) const
    {
        return lhs.cost > rhs.cost;
    }
};

uint32_t d18_dijk_path(const map<char, vector<const d18_edge*>>& nodes)
{
    map<pair<char, uint32_t>, d18_blop> blops;
    auto newnode = blops.try_emplace(make_pair('@', 0), '@', nullptr, 0, 0, nodes);

    priority_queue<d18_blop*, vector<d18_blop*>, d18_blop_prio_cmp> q;
    q.push(&newnode.first->second);

    size_t desired_len = nodes.size() - 1;
    uint32_t desired_bits = (1 << desired_len) - 1;

    while (!q.empty())
    {
        d18_blop* pcurr = q.top();
        q.pop();
        if (pcurr->visited)
            continue;

        if (pcurr->keys_owned == desired_bits)
        {
            cout << " ## found shortest path: ";
            for (const d18_blop* pb = pcurr; pb; pb = pb->parent)
            {
                if (pb != pcurr) cout << " <- ";
                cout << pb->key;
            }
            cout << endl;

            return pcurr->cost;
        }

        pcurr->visited = true;

        for (auto& n : pcurr->possible)
        {
            uint32_t nbit = 1 << (n.first - 'a');

            uint32_t new_cost = pcurr->cost + n.second;
            uint32_t new_ko = pcurr->keys_owned | nbit;

            auto it_created = blops.try_emplace(make_pair(n.first, new_ko), n.first, pcurr, new_cost, new_ko, nodes);
            auto itnode = it_created.first;
            if (it_created.second)
            {
                q.push(&itnode->second);
            }
            else if (new_cost < itnode->second.cost)
            {
                _ASSERT(new_ko == itnode->second.keys_owned);    // this must always match the one in the key

                                                                 // update the cost of the node and re-add it to the prio q
                d18_blop& blop = itnode->second;
                blop.cost = new_cost;
                blop.parent = pcurr;

                // hacky way to update the prio queue o_O  (from https://stackoverflow.com/a/5811888)
                make_heap(const_cast<d18_blop**>(&q.top()),
                    const_cast<d18_blop**>(&q.top()) + q.size(),
                    d18_blop_prio_cmp());
            }
        }
    }

    return 0;
}

int day18(const stringlist& input)
{
    d18_map2d m(input);

    // find all the interesting places
    map<char, d18_pt> items;
    for (char key = 'a'; key <= 'z'; ++key)
    {
        auto pt = m.find_item(key);
        if (pt.x >= 0)
            items.emplace(key, pt);
    }
    items['@'] = m.find_item('@');

    // now build a graph using the keys as node
    vector<d18_edge> edges;
    edges.reserve(items.size() * (items.size() - 1));
    for (auto ita = items.begin(); ita != items.end(); ++ita)
    {
        auto itb = ita;
        ++itb;
        for (; itb != items.end(); ++itb)
        {
            d18_edge e;
            e.ends[0] = ita->first;
            e.ends[1] = itb->first;

            int dist = m.try_path(ita->second, itb->second, &e.keys_needed);
            _ASSERT(dist > 0);
            e.cost = (uint16_t)dist;

            edges.push_back(move(e));
        }
    }

    // and a quick lookup into that
    map<char, vector<const d18_edge*>> nodes;
    for (auto it_e = edges.begin(); it_e != edges.end(); ++it_e)
    {
        d18_edge* pe = &*it_e;

        for (int end = 0; end < 2; ++end)
        {
            // don't add anything returning to the start point
            if (end == 1 && pe->ends[0] == '@')
                continue;

            auto it = nodes.find(pe->ends[end]);
            if (it == nodes.end())
                nodes.emplace(pe->ends[end], vector<const d18_edge*>{ pe });
            else
                it->second.push_back(pe);
        }
    }

    return (int)d18_dijk_path(nodes);
}
