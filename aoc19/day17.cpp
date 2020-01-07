#include <algorithm>
#include <iterator>

#include "harness.h"
#include "intproc.h"
#include "util.h"


typedef pt2d<int16_t> d17_pt;

struct d17_2dmap
{
    size_t width;
    size_t height;
    vector<char> tiles;

    d17_2dmap(const stringlist& lines)
    {
        width = lines.front().size() + 2;
        height = lines.size() + 2;
        _ASSERT(width < 0xffff && height < 0xffff);

        tiles.reserve(width * height);

        // pad the top
        fill_n(back_inserter(tiles), width, '.');
        for (const auto& line : lines)
        {
            tiles.push_back('.');   // pad left
            copy(line.begin(), line.end(), back_inserter(tiles));
            tiles.push_back('.');   // pad right
        }
        // pad the bottom
        fill_n(back_inserter(tiles), width, '.');
    }

    d17_pt find_robot() const
    {
        const string bot("^<>v");
        auto it = find_first_of(tiles.begin(), tiles.end(), bot.begin(), bot.end());
        auto index = distance(tiles.begin(), it);

        return d17_pt((int16_t)(index % width), (int16_t)(index / width));
    }

    char get(size_t x, size_t y) const
    {
        return tiles[x + y*width];
    }
    char get(const d17_pt& p) const
    {
        return tiles[p.x + p.y*width];
    }
};
ostream& operator<<(ostream& os, const d17_2dmap& m)
{
    auto it = m.tiles.begin();
    for (size_t y = 0; y < m.height; ++y, it += m.width)
    {
        os.write(&*it, m.width);
        os << '\n';
    }
    return os;
}

void d17_disasm(const string& program)
{
    ip_disasm(program, "17", IntProcSymbols({
        { 330, "const_zero" },
        { 331, "const_one" },
        { 332, "use_intro_mode" },
        { 570, "tmp" },
        { 571, "flipflip_pc36" },
        { 575, "continuous_video_enable" },
        { 1403, "bss" },
    },
    {
        { 579, "execute" },
    }));
}

int d17_sum_alignments(const d17_2dmap& m)
{
    size_t total = 0;
    // NB: no intersections on outside edge
    for (size_t y = 1; y < m.height - 1; ++y)
    {
        auto it = m.tiles.begin() + (y * m.width) + 1;
        for (size_t x = 1; x < m.width - 1; ++x, ++it)
        {
            if ((*it != '#') ||
                (*(it - 1) != '#') ||
                (*(it + 1) != '#') ||
                (*(it - m.width) != '#') ||
                (*(it + m.width) != '#'))
                continue;

            size_t alignment = (x - 1) * (y - 1);   // -1 for padding
            total += alignment;
        }
    }

    return (int)total;
}

int d17_sum_alignments(const stringlist& input)
{
    return d17_sum_alignments(d17_2dmap(input));
}

void d17_extract_map(const string& program, stringlist& out_lines)
{
    IntProc ip(program);
    ip.run();
    string str_out = ip.read_output_string();
    str_out.erase(str_out.find_last_not_of("\n\r") + 1);
    out_lines = stringlist::fromstring(str_out);
}

int day17(const string& program)
{
    stringlist lines;
    d17_extract_map(program, lines);
    d17_2dmap m(lines);

    // write out the map
    {
        ofstream ofs("data/day17_map.txt");
        ofs << m << endl;
    }

    return d17_sum_alignments(m);
}

int d17_dir_from_chr(char bot)
{
    switch (bot)
    {
    case '^': return 0;
    case '>': return 1;
    case 'v': return 2;
    case '<': return 3;
    }
    throw "Bad Robot!";
}
static const vector<d17_pt> d17_dirs{ { 0,-1 },{ 1,0 },{ 0,1 },{ -1,0 } };

string d17_build_full_route(const d17_2dmap& m)
{
    ostringstream route;

    d17_pt pos = m.find_robot();
    int dir = d17_dir_from_chr(m.get(pos));

    for (;;)
    {
        const d17_pt dir_vec = d17_dirs[dir];

        // try to move all the way forwards
        int dist = 0;
        while (m.get(pos + dir_vec) == '#')
        {
            pos += dir_vec;
            ++dist;
        }
        if (dist > 0)
            route << dist << ',';

        // now try to turn to find more path
        int ldir = (dir + 3) & 3;
        if (m.get(pos + d17_dirs[ldir]) == '#')
        {
            dir = ldir;
            route << "L,";
            continue;
        }
        int rdir = (dir + 1) & 3;
        if (m.get(pos + d17_dirs[rdir]) == '#')
        {
            dir = rdir;
            route << "R,";
            continue;
        }

        // nope, i guess that was it!
        break;
    }

    // NB: this has a trailing comma, but that might be useful for common feature analysis
    return route.str();
}

string replace_all(const string& haystack, const string& needle, const string& replacement)
{
    string out(haystack);
    auto nlen = needle.size();

    for (;;)
    {
        auto it = find_end(out.begin(), out.end(), needle.begin(), needle.end());
        if (it == out.end())
            return out;

        out.replace(it, it + nlen, replacement);
    }
}

struct d17_program
{
    string master;
    string a, b, c;
    bool clean;

    d17_program() : clean(false) {/**/ }


    void remove_trailing_comma(string& s)
    {
        if (s.back() == ',')
            s.erase(s.end() - 1);
    }
    void cleanup()
    {
        remove_trailing_comma(master);
        remove_trailing_comma(a);
        remove_trailing_comma(b);
        remove_trailing_comma(c);
        clean = true;
    }

    bool is_length_valid() const
    {
        size_t max_len = 20;
        if (!clean)
            max_len++;

        if (master.size() > max_len ||
            a.size() > max_len ||
            b.size() > max_len ||
            c.size() > max_len)
        {
            return false;
        }

        return true;
    }

    bool is_master_valid() const
    {
        return (master.find_first_not_of("ABC,") == string::npos);
    }

    bool is_valid() const
    {
        return is_length_valid() && is_master_valid();
    }
};

d17_program d17_factorise(const string& full_route)
{
    d17_program curr;
    curr.master = full_route;

    d17_program best = curr;

    size_t shortest_a = 0;
    for (;;)
    {
        // assume we have a repeated section starting at pos 0, go find it
        auto start_a = full_route.begin();
        auto repeat_a = full_route.end();
        auto len_a = shortest_a ? (shortest_a - 1) : (full_route.size() / 2);
        for (; len_a > 1; --len_a)
        {
            auto end_a = start_a + len_a;
            repeat_a = search(end_a + 1, full_route.end(), start_a, end_a);
            if (repeat_a != full_route.end())
            {
                shortest_a = len_a;
                break;
            }
        }
        if (len_a == 1)
            break;

        curr.a = move(full_route.substr(0, len_a));
        //cout << "Found potential A: '" << a << "' repeated at " << distance(full_route.begin(), repeat_a) << endl;
        string master(move(replace_all(full_route, curr.a, "A,")));
        //cout << "   .. master: '" << master << "'\n";

        // assume we have a repeated section starting just after A,B,
        auto start_b = master.begin() + master.find_first_not_of("A,");
        auto len_b = master.size() / 2;
        for (; len_b > 1; --len_b)
        {
            auto end_b = start_b + len_b;
            auto repeat_b = search(end_b + 1, master.end(), start_b, end_b);
            if (repeat_b == master.end())
                continue;

            // any subroutine needs to end with comma
            if (*(end_b - 1) != ',')
                continue;

            // it's invalid to call other movement routines
            if (find(start_b, end_b, 'A') != end_b)
                continue;

            curr.b = move(string(start_b, end_b));
            //cout << "Found potential B: '" << b << "'\n";
            string master_b(move(replace_all(master, curr.b, "B,")));
            //cout << "   .. master: '" << master_b << "'\n";

            // assume we have a repeated section starting just after A,B,
            auto start_c = master_b.begin() + master_b.find_first_not_of("AB,");
            auto len_c = master_b.size() / 2;
            for (; len_c > 1; --len_c)
            {
                auto end_c = start_c + len_c;
                auto repeat_c = search(end_c + 1, master_b.end(), start_c, end_c);
                if (repeat_c == master_b.end())
                    continue;

                // any subroutine needs to end with comma
                if (*(end_c - 1) != ',')
                    continue;

                // it's invalid to call other movement routines
                static const string AB = "AB";
                if (find_first_of(start_c, end_c, AB.begin(), AB.end()) != end_c)
                    continue;

                curr.c = move(string(start_c, end_c));
                //cout << "Found potential C: '" << c << "'\n";
                curr.master = move(replace_all(master_b, curr.c, "C,"));
                //cout << "   .. master: '" << master_c << "'\n";

                if (curr.is_valid() || curr.master.size() < best.master.size())
                {
                    best = curr;
                    best.cleanup();

                    //cout << "Better:\n"
                    //    "  MSTR: " << best.master << '\n' <<
                    //    "     A: " << best.a << '\n' <<
                    //    "     B: " << best.b << '\n' <<
                    //    "     C: " << best.c << endl;

                    if (best.is_valid())
                    {
                        cout << "Found a valid program:\n"
                            "  MSTR: " << best.master << '\n' <<
                            "     A: " << best.a << '\n' <<
                            "     B: " << best.b << '\n' <<
                            "     C: " << best.c << endl;

                        return best;
                    }
                }
            }
        }
    }

    cout << "o no, couldn't find a valid program o_O" << endl;
    return best;
}

int64_t d17_follow_route(const string& program, const d17_program& route)
{
    IntProc ip(program);

    // engage "follow program" mode
    ip.poke(0, 2);

    char video_feed = false;

    // input our program
    ostringstream input_os;
    input_os << route.master << '\n' << route.a << '\n' << route.b << '\n' << route.c << '\n' << (video_feed ? 'y' : 'n') << '\n';
    string input_str(move(input_os.str()));
    list<IntProc::word_t> input(input_str.begin(), input_str.end());
    ip.set_input(input);

    ip.run();

    auto result = ip.read_final_output();

    if (video_feed)
    {
        ofstream ofs("data/day17_video.txt");
        ofs << ip.read_output_string();
    }

    return result;
}

int64_t day17_2(const string& program)
{
    stringlist lines;
    d17_extract_map(program, lines);
    d17_2dmap m(lines);

    string full_route = d17_build_full_route(m);
    //cout << full_route << endl;

    d17_program route = d17_factorise(full_route);

    return d17_follow_route(program, route);
}


