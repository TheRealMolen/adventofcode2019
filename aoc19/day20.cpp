#include "harness.h"
#include "util.h"

#include <iterator>
#include <queue>
#include <vector>


typedef pt2d<int16_t> d20_pt;
enum compass { NORTH = 0, EAST, SOUTH, WEST };
static const vector<d20_pt> d20_dirs = { { 0,-1 },{ 1,0 },{ 0,1 },{ -1,0 } }; // N E S W

struct d20_portal
{
    char name[3];
    int8_t exit_dir;
    bool is_outside;
    bool has_pair;
    d20_pt pos;

    d20_portal(size_t x, size_t y, compass exit, bool outside, char a, char b)
        : exit_dir((int8_t)exit)
        , is_outside(outside)
        , pos((int16_t)x, (int16_t)y)
    {
        name[0] = a;
        name[1] = b;
        name[2] = 0;

        // AA and ZZ are special - they have no paired portal, so they're only accessible at the top layer of the doughnut
        has_pair = !((a == 'A' && b == 'A') || (a == 'Z' && b == 'Z'));
    }

    bool operator==(const char* oname) const
    {
        _ASSERT(oname);
        return name[0] == oname[0] && name[1] == oname[1];
    }
    bool operator==(const d20_pt& opos) const
    {
        return pos.x == opos.x && pos.y == opos.y;
    }
    bool operator==(const d20_portal& o) const
    {
        return *this == o.pos;
    }
    bool operator!=(const d20_portal& o) const
    {
        return !(*this == o.pos);
    }
};

struct d20_map2d
{
    size_t width;
    size_t height;
    vector<char> tiles;
    vector<d20_portal> portals;

    d20_map2d(const stringlist& lines);

    char get(size_t x, size_t y) const
    {
        return tiles[x + y*width];
    }
    char get(const d20_pt& p) const
    {
        return tiles[p.x + p.y*width];
    }

    size_t calc_ix(const d20_pt& p) const
    {
        return p.x + p.y * width;
    }
    d20_pt pt_from_ix(size_t ix) const
    {
        return d20_pt((int16_t)(ix % width), (int16_t)(ix / width));
    }

    d20_pt get_start_point() const
    {
        auto it_portal = find(portals.begin(), portals.end(), "AA");
        if (it_portal == portals.end())
        {
            throw "couldn't find AA portal o_O";
        }
        return it_portal->pos + d20_dirs[it_portal->exit_dir];
    }
    d20_pt get_end_point() const
    {
        auto it_portal = find(portals.begin(), portals.end(), "ZZ");
        if (it_portal == portals.end())
        {
            throw "couldn't find ZZ portal o_O";
        }
        return it_portal->pos + d20_dirs[it_portal->exit_dir];
    }

    int try_path(const d20_pt& from, const d20_pt& to) const;
    int try_path_doughnut() const;  // paths from AA to ZZ according to doughnut recursion rules
};
ostream& operator<<(ostream& os, const d20_map2d& m)
{
    auto it = m.tiles.begin();
    for (size_t y = 0; y < m.height; ++y, it += m.width)
    {
        os.write(&*it, m.width);
        os << '\n';
    }
    return os;
}


d20_map2d::d20_map2d(const stringlist& lines)
{
    width = lines.front().size();
    height = lines.size();

    tiles.reserve(width * height);

    for (const auto& line : lines)
    {
        _ASSERT(line.size() == width);
        copy(line.begin(), line.end(), back_inserter(tiles));
    }

    // now go hunting for portals!
    auto it = tiles.begin();
    for (size_t y = 0; y + 1 < height; ++y)
    {
        for (size_t x = 0; x + 1 < width; ++x, ++it)
        {
            char c = *it;
            if (c < 'A' || c > 'Z')
                continue;

            // is this portal on the inside or outside of the doughnut?
            bool outside = (y < 2) || (y >= height - 2) || (x < 2) || (x >= width - 2);

            // the rest of the portal name is to the right or below this char (or we've already processed it)
            char n = *(it + 1);
            if (n >= 'A' && n <= 'Z')
            {
                compass exit = ((x>0) && *(it - 1) == '.') ? WEST : EAST;
                size_t port_x = (exit == WEST) ? x : x + 1;
                portals.emplace_back(port_x, y, exit, outside, c, n);
            }
            else
            {
                n = *(it + width);
                if (n >= 'A' && n <= 'Z')
                {
                    compass exit = ((y>0) && *(it - width) == '.') ? NORTH : SOUTH;
                    size_t port_y = (exit == NORTH) ? y : y + 1;
                    portals.emplace_back(x, port_y, exit, outside, c, n);
                }
            }
        }

        // because we skip the last column, we need to move our iterator along or we go out of sync
        ++it;
    }
}

int d20_map2d::try_path(const d20_pt& from, const d20_pt& to) const
{
    // indexed same as tiles
    vector<bool> visited(tiles.size(), false);
    vector<uint16_t> distances(tiles.size(), 0xffffu);

    // start point has distance of zero
    distances[calc_ix(from)] = 0;

    vector<size_t> unvisited_by_ix;
    unvisited_by_ix.reserve(tiles.size());
    unvisited_by_ix.push_back(calc_ix(from));

    while (!unvisited_by_ix.empty())
    {
        // find unvisited with the lowest distance
        auto it_curr_ix = min_element(unvisited_by_ix.begin(), unvisited_by_ix.end(),
            [&](auto ix_a, auto ix_b) { return distances[ix_a] < distances[ix_b]; });
        auto curr_ix = *it_curr_ix;
        unvisited_by_ix.erase(it_curr_ix);

        auto curr_p = pt_from_ix(curr_ix);
        _ASSERT(curr_p != to);

        // there may be dupes in the unvisited list
        if (visited[curr_ix])
            continue;
        visited[curr_ix] = true;

        auto curr_dist = distances[curr_ix];

        uint16_t new_ndist = curr_dist + 1;

        // spin through unvisited neighbours and update their distances if need be
        for (auto& dir : d20_dirs)
        {
            auto n = curr_p + dir;
            if (n == to)
            {
                //cout << "reached the end!!\n";
                //auto it_dist = distances.begin();
                //auto it_t = tiles.begin();
                //for (size_t oy = 0; oy < height; ++oy)
                //{
                //    for (size_t ox = 0; ox < width; ++ox, ++it_dist, ++it_t)
                //    {
                //        int d = *it_dist;
                //        if (d != 0xffff)
                //            cout << (char)((d % 26) + 'a');
                //        else
                //            cout << *it_t;
                //    }
                //    cout << endl;
                //}

                return new_ndist;
            }

            size_t nindex = calc_ix(n);
            if (visited[nindex])
                continue;

            auto ntile = tiles[nindex];
            if (ntile == '#' || ntile == ' ')
                continue;

            // have we found a portal?
            if (ntile >= 'A' && ntile <= 'Z')
            {
                // find the portal for this spot
                auto it_portal = find(portals.begin(), portals.end(), n);
                if (it_portal == portals.end())
                    continue;

                // find the other end of the portal
                auto it_dest = find_if(portals.begin(), portals.end(), [&](const auto& p)
                {
                    return p != *it_portal && p == it_portal->name;
                });
                if (it_dest == portals.end())
                    continue;

                // teleport us to the other end
                n = it_dest->pos + d20_dirs[it_dest->exit_dir];
                nindex = calc_ix(n);
            }

            if (new_ndist < distances[nindex])
            {
                distances[nindex] = new_ndist;
                unvisited_by_ix.push_back(nindex);
            }
        }
    }

    return -1;
}


struct d20_doughnut_context
{
    vector<bool> visited;
    vector<uint32_t> distances;

    d20_doughnut_context(const d20_map2d& m)
        : visited(m.tiles.size(), false)
        , distances(m.tiles.size(), 0xffffffffu)
    { /**/
    }
};

struct d20_dijk
{
    d20_pt pos;
    uint16_t layer;
    uint32_t cost;

    d20_dijk(const d20_pt& p, uint16_t l, uint32_t c) : pos(p), layer(l), cost(c) {/**/ }
};

struct d20_dijk_prio_cmp
{
    bool operator()(const d20_dijk* lhs, const d20_dijk* rhs) const { return lhs->cost > rhs->cost; }
    bool operator()(const d20_dijk& lhs, const d20_dijk& rhs) const { return lhs.cost > rhs.cost; }
};

int d20_map2d::try_path_doughnut() const
{
    vector<d20_doughnut_context> layers;
    layers.reserve(100);

    layers.emplace_back(*this);

    auto from = get_start_point();
    auto to = get_end_point();

    priority_queue<d20_dijk, vector<d20_dijk>, d20_dijk_prio_cmp> q;

    // start point has distance of zero
    layers[0].distances[calc_ix(from)] = 0;
    q.emplace(from, 0, 0);

    while (!q.empty())
    {
        d20_dijk curr = q.top();
        q.pop();

        auto curr_ix = calc_ix(curr.pos);

        d20_doughnut_context* p_layer = &layers[curr.layer];

        // there may be dupes in the unvisited list
        if (p_layer->visited[curr_ix])
            continue;
        p_layer->visited[curr_ix] = true;

        _ASSERT(p_layer->distances[curr_ix] >= curr.cost);
        p_layer->distances[curr_ix] = curr.cost;

        uint32_t new_ncost = curr.cost + 1;

        // spin through unvisited neighbours and update their distances if need be
        for (auto& dir : d20_dirs)
        {
            auto n = curr.pos + dir;
            if (curr.layer == 0 && n == to)
            {
                //cout << "reached the end!!\n";
                //auto it_dist = distances.begin();
                //auto it_t = tiles.begin();
                //for (size_t oy = 0; oy < height; ++oy)
                //{
                //    for (size_t ox = 0; ox < width; ++ox, ++it_dist, ++it_t)
                //    {
                //        int d = *it_dist;
                //        if (d != 0xffff)
                //            cout << (char)((d % 26) + 'a');
                //        else
                //            cout << *it_t;
                //    }
                //    cout << endl;
                //}

                return (int)new_ncost;
            }

            size_t nindex = calc_ix(n);
            if (p_layer->visited[nindex])
                continue;

            auto ntile = tiles[nindex];
            if (ntile == '#' || ntile == ' ')
                continue;

            size_t nlayer = curr.layer;

            // have we found a portal?
            if (ntile >= 'A' && ntile <= 'Z')
            {
                // find the portal for this spot
                auto it_portal = find(portals.begin(), portals.end(), n);
                if (it_portal == portals.end())
                    continue;

                // if this is an exit portal and we're not at the outermost layer, it's a wall
                if (!it_portal->has_pair && nlayer != 0)
                    continue;

                // if this is an outside portal and we're at the outermost layer, it's a wall
                if (it_portal->is_outside && nlayer == 0)
                    continue;

                // find the other end of the portal
                auto it_dest = find_if(portals.begin(), portals.end(), [&](const auto& p)
                {
                    return p != *it_portal && p == it_portal->name;
                });
                if (it_dest == portals.end())
                    continue;

                // adjust our layer
                nlayer += it_portal->is_outside ? -1 : 1;

                // make sure it exists!
                if (nlayer >= layers.size())
                {
                    layers.emplace_back(*this);
                    // this may have reallocated our layers array, so make sure we update p_layer
                    p_layer = &layers[curr.layer];
                }

                // teleport us to the other end
                n = it_dest->pos + d20_dirs[it_dest->exit_dir];
                nindex = calc_ix(n);
            }

            if (new_ncost < layers[nlayer].distances[nindex])
            {

                layers[nlayer].distances[nindex] = new_ncost;
                q.emplace(n, (uint16_t)nlayer, new_ncost);
            }
        }
    }

    return -1;
}


int day20(const stringlist& input)
{
    d20_map2d m(input);

    auto start = m.get_start_point();
    auto end = m.get_end_point();
    auto dist = m.try_path(start, end);

    return dist;
}

int day20_2(const stringlist& input)
{
    d20_map2d m(input);

    return m.try_path_doughnut();
}
