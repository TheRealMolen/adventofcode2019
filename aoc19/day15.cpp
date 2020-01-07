#include "harness.h"
#include "intproc.h"
#include "util.h"


typedef pt2d<int16_t> d15_pt;


void d15_disasm(const string& program)
{
    ip_disasm(program, "15", IntProcSymbols({
        { 1032, "t" },
        { 1033, "command" },
        { 1034, "curr_x" },
        { 1035, "curr_y" },
        { 1036, "curr_x_odd" },
        { 1037, "curr_halfy" },
        { 1038, "curr_y_odd" },
        { 1039, "new_x" },
        { 1040, "new_y" },
        { 1041, "new_x_odd" },
        { 1042, "new_halfy" },
        { 1043, "new_y_odd" },
        { 1044, "result" },
    },
    {
        { 31, "go_north" },
        { 58, "go_south" },
        { 81, "go_west" },
        { 104, "go_east" },
        { 124, "do_move" },
        { 165, "check_tile" },
        { 217, "out_of_bounds" },
        { 224, "end_move" },
        { 247, "output_result" },
    }));
}

struct d15_map
{
    size_t width;
    size_t height;
    d15_pt pos;
    vector<int8_t> tiles;   // -1 = untested, 0 = wall, 1 = clear, 2 = console, 3 = start
    vector<uint16_t> costs;

    d15_map(size_t size) : width(size), height(size), pos((int16_t)size / 2, (int16_t)size / 2)
    {
        tiles.resize(width*height, -1);
        costs.resize(width*height, 0xffffu);

        set(pos.x, pos.y, 3);
        setcost(pos.x, pos.y, 0);
    }

    int8_t get(size_t x, size_t y) const
    {
        if (x > width || y > height)
        {
            return 0;
        }

        return tiles[x + y * width];
    }

    void set(const d15_pt& p, int8_t val)
    {
        tiles[p.x + p.y * width] = val;
    }
    void set(size_t x, size_t y, int8_t val)
    {
        tiles[x + y * width] = val;
    }
    void setcost(size_t x, size_t y, uint16_t val)
    {
        costs[x + y * width] = val;
    }
};

ostream& operator<<(ostream& os, const d15_map& m)
{
    auto it = m.tiles.begin();
    for (size_t y = 0; y < m.height; ++y)
    {
        for (size_t x = 0; x < m.width; ++x, ++it)
        {
            if (x == m.pos.x && y == m.pos.y)
            {
                os << 'o';
                continue;
            }

            auto t = *it;
            if (t < 0)
                os << '.';
            else if (t == 0)
                os << '#';
            else if (t == 1)
                os << ' ';
            else if (t == 2)
                os << 'k';
            else if (t == 3)
                os << 'x';
            else if (t == 10)
                os << 'O';
            else
                os << '!';
        }
        os << '\n';
    }

    return os;
}

static const vector<d15_pt> d15_moves = { { 0,0 },{ 0,-1 },{ 0,1 },{ -1,0 },{ 1,0 } };

struct d15_node
{
    d15_pt pos;
    vector<int8_t> open;
    int8_t return_move;

    void try_add_move(int8_t move, const d15_map& map)
    {
        auto npos = pos + d15_moves[move];
        if (map.get(npos.x, npos.y) == -1)
            open.push_back(move);
    }

    d15_node(d15_pt _pos, int8_t move_here, const d15_map& map) : pos(_pos)
    {
        try_add_move(1, map);
        try_add_move(2, map);
        try_add_move(3, map);
        try_add_move(4, map);

        return_move = ((move_here - 1) ^ 1) + 1;
    }
};


int8_t d15_check_tile(int x, int y, const IntProc& ip)
{
    if (x == 0 || y == 0)
        return 0;
    if (x == 40 || y == 40)
        return 0;

    if (x == 35 && y == 9)
        return 2;

    int x_odd = x & 1;
    int y_odd = y & 1;
    if (x_odd * y_odd != 0)
        return 1;
    if (x_odd + y_odd == 0)
        return 0;

    int t = ((y / 2) + y_odd - 1);
    t *= 39;
    t += x - 1;

    int n = (int)ip.peek(252 + t);
    return (n < 26) ? 1 : 0;
}

d15_map d15_extract_map(const IntProc& ip)
{
    auto size = ip.peek(139) + 1;
    d15_map smap(size);

    for (int16_t y = 0; y < size; ++y)
    {
        for (int16_t x = 0; x < size; ++x)
        {
            smap.set({ x,y }, d15_check_tile(x, y, ip));
        }
    }

    // cout << "------ hax0red version of map vvvvvv ----------------\n" << smap << "------------- ^^^^^^ --------------\n" << endl;
    return smap;
}

int day15(const string& program)
{
    IntProc ip(program);

    d15_map smap(50);
    vector<d15_node> moves;
    moves.emplace_back(smap.pos, -1, smap);

    // go full exhaustive
    while (!moves.empty())
    {
        d15_node& move = moves.back();

        int command = move.open.back();
        move.open.pop_back();

        ip.set_input({ command });
        ip.run();

        auto result = ip.read_output();
        auto wpos = smap.pos + d15_moves[command];
        smap.set(wpos, (int8_t)result);

        if (result)
        {
            if (result == 2)
                return (int)moves.size();

            smap.pos = wpos;
            moves.emplace_back(wpos, command, smap);
        }

        while (!moves.empty() && moves.back().open.empty())
        {
            ip.set_input({ moves.back().return_move });
            if (!ip.run())
            {
                wpos = smap.pos + d15_moves[moves.back().return_move];
                smap.pos = wpos;

#ifdef _DEBUG
                auto res2 =
#endif
                    ip.read_output();
                _ASSERT(res2 != 0);
            }
            moves.pop_back();
        }
    }

    cout << smap << endl;

    return -1;
}

#ifdef D_INTERACTIVE_15
//..................................................        ------hax0red versin of map vvvvvv-------
//.....###.###.#############.#####.#######.###......        #########################################
//....#   #   #             #     #       #   #.....        #   #   #             #     #       #   #
//....# # # # ######### ### # ### ##### # ### #.....        # # # # ######### ### # ### ##### # ### #
//....# #   #         # #     # #       #     #.....        # #   #         # #     # #       #     #
//....# ############# # ####### ############# #.....        # ############# # ####### ############# #
//....#           #     #   #     #       #   #.....        #           #     #   #     #       #   #
//....# ######### ### ### # # # ### ##### # ##......        # ######### ### ### # # # ### ##### # ###
//....# #       #   # #   # # #     #   #   # #.....        # #       #   # #   # # #     #   #   # #
//....# # ##### ### ### ### # ####### # ##### #.....        # # ##### ### ### ### # ####### # ##### #
//....#   #   # # #       # #         # #k    #.....        #   #   # # #       # #         # #k    #
//.....###### # # ######### ########### ##### #.....        ####### # # ######### ########### ##### #
//....#       # #     #     #         # #   # #.....        #       # #     #     #         # #   # #
//....# # ### # # ### # ##### ##### # # # # # #.....        # # ### # # ### # ##### ##### # # # # # #
//....# #   # #   #   # #   #     # # #   # # #.....        # #   # #   #   # #   #     # # #   # # #
//....# ### # ### ##### # # ##### # # ##### # #.....        # ### # ### ##### # # ##### # # ##### # #
//....#   # #   # #     # #   #   # #     # # #.....        #   # #   # #     # #   #   # #     # # #
//.....## # ### # # ##### ##### ### # ##### # #.....        ### # ### # # ##### ##### ### # ##### # #
//....# # # #   # #     #       # # # #     # #.....        # # # # x # #     #       # # # #     # #
//....# # # ##### ##### ####### # # ### ##### #.....        # # # ##### ##### ####### # # ### ##### #
//....# # #     # #   #   #   #   #   # #     #.....        # # #     # #   #   #   #   #   # #     #
//....# # ##### # # ##### # # # ##### # # ### #.....        # # ##### # # ##### o # # ##### # # ### #
//....#   #   #   #     # #o# # # - #   #   # #.....        #   #   #   #     # # # # #   #   #   # #
//....# ### # ##### # # # ### ### # # ##.## # #..... +      # ### # ##### # # # ### ### # # ##### # #
//....# #   #   #   # # #   #     # #   #   # #.....        # #   #   #   # # #   #     # #   #   # #
//....# ##### # ##### ### # ####### ### # ### #.....        # ##### # ##### ### # ####### ### # ### #
//....# #     #     # #   # #     #   #   #   #.....        # #     #     # #   # #     #   #   #   #
//....# # ######### # # ##### ### ### ### ####......        # # ######### # # ##### ### ### ### #####
//....#   #   ┼   # # #     # #     # #   #   #.....        #   #       # # #     # #     # #   #   #
//.....#### # ##### # ##### # # ### # ##### # #.....        ##### # ##### # ##### # # ### # ##### # #
//....# #   #     #       #   #   # #       # #.....        # #   #     #       #   #   # #       # #
//....# # ####### ####### ####### ##.######## #.....        # # ####### ####### ####### ########### #
//....# #     # #       # #     #   #         #.....        # #     # #       # #     #   #         #
//....# ##### # ####### # ##### ### # ########......        # ##### # ####### # ##### ### # #########
//....# #   #     #   # # #   #   #   #       #.....        # #   #     #   # # #   #   #   #       #
//....# # # ##### # # # # # # ### ######### # #.....        # # # ##### # # # # # # ### ######### # #
//....#   #     #   # # # # #           #   # #.....        #   #     #   # # # # #           #   # #
//....# ### ######### # # # ########### # ### #.....        # ### ######### # # # ########### # ### #
//....#   #         # #   # #     #     # # # #.....        #   #         # #   # #     #     # # # #
//.....## ##### ##### ##### # ### # ##### # # #.....        ### ##### ##### ##### # ### # ##### # # #
//....#       #             #   #         #   #..... +      #       #             #   #         #   #
//.....#######.#############.###.#########.###......        #########################################

int day15_i(const string& program)
{
    // useful to have disasm around!
    //d15_disasm(program);

    IntProc ip(program);
    d15_map smap(50);

    for (;;)
    {
        cout << smap << endl;

        int c = _getch();
        int command = 0;
        switch (c)
        {
        case 27: return 0;

        case 'k': command = 1; break;
        case 'j': command = 2; break;
        case 'h': command = 3; break;
        case 'l': command = 4; break;
        default: continue;
        }

        ip.set_input({ command });
        ip.run();

        auto result = ip.read_output();
        auto wpos = smap.pos + d15_moves[command];
        smap.set(wpos, (int8_t)result);

        if (result)
            smap.pos = wpos;
    }

    return -1;
}
#endif


int day15_2(const string& program)
{
    IntProc ip(program);
    d15_map smap(move(d15_extract_map(ip)));

    // change the k to an O
    auto it_console = find(smap.tiles.begin(), smap.tiles.end(), 2);
    *it_console = 10;

    d15_map nmap = smap;
    int nsteps = 0;
    bool changed;
    do {
        nsteps++;
        changed = false;

        for (int y = 1; y < 40; ++y)
        {
            auto it_o = smap.tiles.begin() + (y * nmap.width + 1);
            auto it_n = nmap.tiles.begin() + (y * nmap.width + 1);
            for (int x = 1; x < 40; ++x, ++it_o, ++it_n)
            {
                if (*it_n == 0 || *it_n == 10)
                    continue;

                if (*(it_o - 1) == 10 || *(it_o + 1) == 10 || *(it_o - smap.width) == 10 || *(it_o + smap.width) == 10)
                {
                    changed = true;
                    *it_n = 10;
                }
            }
        }

        smap.tiles = nmap.tiles;
    } while (changed);

    // nothing changed on the final step
    nsteps--;

    //cout << "\n after " << nsteps << ":\n\n" << smap << "\n" << endl;
    return nsteps;
}