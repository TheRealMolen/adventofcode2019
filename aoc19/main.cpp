#include "harness.h"
#include "util.h"

#include <algorithm>
#include <boost/circular_buffer.hpp>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>


#define D_REALLY_PLAYx
#define D_INTERACTIVE_15x


#if defined( D_REALLY_PLAY) || defined(D_INTERACTIVE_15)
#include <conio.h>
#endif


#include "intproc.h"

static const float kPI = 3.14159265358979323846f;


int day1(const stringlist& module_sizes);
int day1_2(const stringlist& module_sizes);

string day2(const string& initial);
size_t day2Hcf(const string& initial, IntProc::word_t noun, IntProc::word_t verb);
IntProc::word_t day2_2(const string& initial);

size_t day3(const stringlist& wires);
size_t day3_2(const stringlist& wires);

bool d4_is_code_valid(int code);
bool d4_is_code_valider(int code);
int day4(int lo, int hi);
int day4_2(int lo, int hi);

int day6(const stringlist& input);
size_t day6_2(const stringlist& input);

int day12(const vector<d12_pt>& input, int steps, bool show = false);
int64_t day12_2(const vector<d12_pt>& input, bool show = false);
int64_t day12_2b(const vector<d12_pt>& input, bool show = false);

int day15(const string& program);
int day15_2(const string& program);

int d17_sum_alignments(const stringlist& input);
int day17(const string& program);
int64_t day17_2(const string& program);

int day18(const stringlist& input);

int day20(const stringlist& input);
int day20_2(const stringlist& input);

// -------------------------------------------------------------------

IntProc::word_t day5(const string& initial)
{
    cout << "INITIALISING T.E.S.T...." << endl;
    IntProc ip(initial);
    ip.run();
    IntProc::word_t out;
    do {
        out = ip.read_output();
    } while (ip.has_output());

    return  out;
}

IntProc::word_t day5_2(const string& initial, IntProc::word_t input)
{
    IntProc ip(initial);
    ip.set_input({ input });
    ip.run();
    return ip.read_output();
}

// -------------------------------------------------------------------

int d7_run_amp(int phase, int input, const vector<IntProc::word_t>& program)
{
    IntProc ip(program);
    ip.set_input({ phase, input });
    ip.run();
    return (int)ip.read_output();
}

int d7_run_amp_chain(const vector<int>& phases, const vector<IntProc::word_t>& program)
{
    int output = 0;
    for( auto p : phases)
    {
        output = d7_run_amp(p, output, program);
    }

    return output;
}

int day7(const string& input)
{
    vector<IntProc::word_t> program;
    IntProc::parse(input, program);

    int max_thrusters = -1;
    vector<int> max_phase;

    vector<int> phases = { 0,1,2,3,4 };
    do {
        int thrusters = d7_run_amp_chain(phases, program);

        if (thrusters > max_thrusters)
        {
            max_phase = phases;
            max_thrusters = thrusters;
        }
    } while (next_permutation(phases.begin(), phases.end()));

    cout << "Max Thrust @ phase setting ";
    for (auto phase : max_phase)
        cout << phase;
    cout << endl;

    return max_thrusters;
}


IntProc::word_t d7_2_run_amp_chain(const vector<int>& phases, const vector<IntProc::word_t>& program)
{
    vector<IntProc> amps;
    for (auto phase : phases)
    {
        amps.emplace_back(program);
        amps.back().set_input({ phase });
    }

    IntProc::word_t output = 0;
    bool finished = false;
    do {
        for (auto& amp : amps)
        {
            amp.append_input(output);
            if (amp.run())
            {
                finished = true;
            }
            output = amp.read_output();
        }
    } while (!finished);

    return output;
}

IntProc::word_t day7_2(const string& input)
{
    vector<IntProc::word_t> program;
    IntProc::parse(input, program);

    IntProc::word_t max_thrusters = -1;
    vector<int> max_phase;

    vector<int> phases = { 5,6,7,8,9 };
    do {
        IntProc::word_t thrusters = d7_2_run_amp_chain(phases, program);

        if (thrusters > max_thrusters)
        {
            max_phase = phases;
            max_thrusters = thrusters;
        }
    } while (next_permutation(phases.begin(), phases.end()));

    cout << "Max Thrust @ phase setting ";
    for (auto phase : max_phase)
        cout << phase;
    cout << endl;

    return max_thrusters;
}

// -------------------------------------------------------------------

int day8(const string& input, int w=25, int h=6)
{
    const int cpl = w * h;

    if ((int)input.size() % cpl)
        throw "bad-sized input";

    int checksum = 0x7ffffff;
    int fewest0 = 0x7ffffff;
    for (auto it = input.begin(); it != input.end(); it += cpl)
    {
        int num0 = (int)count(it, it + cpl, '0');
        if (num0 < fewest0)
        {
            int num1 = (int)count(it, it + cpl, '1');
            int num2 = (int)count(it, it + cpl, '2');
            checksum = num1 * num2;
            fewest0 = num0;
        }
    }

    return checksum;
}

int day8_2(const string& input, int w = 25, int h = 6)
{
    const int cpl = w * h;

    if ((int)input.size() % cpl)
        throw "bad-sized input";
    auto nlayers = input.size() / cpl;

    auto it = input.begin();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x, ++it)
        {
            auto itc = it;
            size_t l = 0;
            for (; l < nlayers && *itc == '2'; ++l, itc += cpl)
            {/**/}

            if (l < nlayers)
            {
                cout << (*itc == '0' ? ' ' : '#');
            }
            else
            {
                cout << 'x';
            }
        }
        cout << endl;
    }

    return 1;
}

// -------------------------------------------------------------------

string ip_run_capture_output(const string& program, const list<IntProc::word_t>& inputs = { 1 })
{
    IntProc ip(program);
    ip.set_input(inputs);
    //ip.dump();
    ip.run();

    ostringstream os;
    bool first = true;
    while (ip.has_output())
    {
        if (!first)
            os << ',';
        first = false;
        os << ip.read_output();
    }
    return os.str();
}


// -------------------------------------------------------------------

typedef pt2d<int16_t> d10_pt;

void d10_read_asteroids(const stringlist& input, set<d10_pt>& out_points)
{
    vector<string> lines(input.begin(), input.end());
    size_t w = lines[0].size();
    size_t h = lines.size();

    for (size_t y = 0; y < h; ++y)
    {
        const string& line = lines[y];
        auto it = line.begin();
        for (size_t x = 0; x < w; ++x, ++it)
        {
            if (*it != '#')
                continue;

            out_points.emplace((int)x, (int)y);
        }
    }
}

string day10(const stringlist& input)
{
    set<d10_pt> points;
    d10_read_asteroids(input, points);

    const d10_pt* best = NULL;
    int most_visible = -1;
    set<d10_pt> done;

    for (auto& pt : points)
    {
        done.clear();
        done.insert(pt);

        int visible = 0;
        for (const d10_pt& test : points)
        {
            if (done.find(test) != done.end())
                continue;
            done.insert(test);

            auto diff = pt - test;
            diff.simplify();
            d10_pt t(test);
            bool blocked = false;
            for (;;)
            {
                t += diff;
                if (t == pt)
                    break;
                if (points.find(t) != points.end())
                {
                    blocked = true;
                    break;
                }
            }

            if (!blocked)
                ++visible;
        }

        if (visible > most_visible)
        {
            most_visible = visible;
            best = &pt;
        }
    }

    ostringstream os;
    os << most_visible << '@' << best->x << ',' << best->y;
    return os.str();
}

struct d10_ast
{
    d10_pt pos;
    float ang;

    d10_ast(const d10_pt& pos, const d10_pt& home) : pos(pos)
    {
        d10_pt diff = pos - home;
        ang = atan2f((float)diff.x, (float)-diff.y);   // intentionally flipped so we're clockwise from north
        if (ang < 0.0f)
            ang += 2.0f * kPI;
    }

    bool operator<(const d10_ast& o) const
    {
        return ang < o.ang;
    }
};


void d10_find_visible(const set<d10_pt> asteroids, const d10_pt& home, set<d10_pt>& out_visible)
{
    out_visible.clear();

    set<d10_pt> done;
    done.insert(home);

    for (const d10_pt& test : asteroids)
    {
        if (done.find(test) != done.end())
            continue;
        done.insert(test);

        auto diff = home - test;
        diff.simplify();
        d10_pt t(test);
        bool blocked = false;
        for (;;)
        {
            t += diff;
            if (t == home)
                break;
            if (asteroids.find(t) != asteroids.end())
            {
                blocked = true;
                break;
            }
        }

        if (!blocked)
            out_visible.insert(test);
    }
}

int day10_2(const stringlist& input, const d10_pt& home, size_t interesting = 1)
{
    set<d10_pt> asteroids;
    d10_read_asteroids(input, asteroids);

    vector<d10_ast> destroyed;
    destroyed.reserve(asteroids.size());

    set<d10_pt> visible;
    vector<d10_ast> targets;
    targets.reserve(asteroids.size());
    bool found = false;
    while (!found && !asteroids.empty())
    {
        // find the current visible set
        d10_find_visible(asteroids, home, visible);

        // build a sortable list
        targets.clear();
        for (auto ast : visible)
            targets.emplace_back(ast, home);

        // sort it :)
        sort(targets.begin(), targets.end());

        // start lasering
        for (auto itboom = targets.begin(); itboom != targets.end(); ++itboom)
        {
            d10_ast& boom = *itboom;
            asteroids.erase(asteroids.find(boom.pos));
            destroyed.emplace_back(boom);

            if (destroyed.size() == interesting)
            {
                found = true;
                break;
            }
        }
    }

    auto& last = destroyed.back();
    return last.pos.x * 100 + last.pos.y;
}


// -------------------------------------------------------------------

int day11(const string& program, bool start_col = false)
{
    IntProc ip(program);

    const size_t hull_hsize = 100;
    const size_t hull_size = 2 * hull_hsize;
    vector<bool> colour;
    vector<bool> painted;
    colour.resize(hull_size * hull_size, false);
    painted.resize(hull_size * hull_size, false);

    d10_pt pos(0, 0);
    d10_pt offs(hull_hsize, hull_hsize);
    d10_pt wpos = pos + offs;
    d10_pt minp = wpos;
    d10_pt maxp = wpos;
    int facing = 0; // up
    const vector<d10_pt> dirs{ { 0, 1}, {-1, 0}, {0, -1}, {1, 0} };

    colour[wpos.x + wpos.y * hull_size] = start_col;
    ip.set_input({ start_col ? 1 : 0 });

    while (!ip.run()) {
        if (ip.has_output())
        {
            // paint & move!
            bool newcol = ip.read_output() != 0;
            colour[wpos.x + wpos.y * hull_size] = newcol;
            painted[wpos.x + wpos.y * hull_size] = true;

            bool turnright = ip.read_output() != 0;
            if (turnright) {
                facing = (facing + 4 - 1) & 3;
            }
            else {
                facing = (facing + 1) & 3;
            }
            pos += dirs[facing];

            wpos = pos + offs;

            minp.x = min(minp.x, wpos.x);
            minp.y = min(minp.y, wpos.y);
            maxp.x = max(maxp.x, wpos.x);
            maxp.y = max(maxp.y, wpos.y);
        }

        bool col = colour[wpos.x + wpos.y * hull_size];
        ip.set_input({ col ? 1 : 0 });
    }

    for (auto y = maxp.y; y >= minp.y; --y)
    {
        auto it = colour.begin() + (minp.x + y * hull_size);
        auto itp = painted.begin() + (minp.x + y * hull_size);
        for (auto x = minp.x; x <= maxp.x; ++x, ++it, ++itp)
        {
            cout << (*it ? '#' : (*itp ? '.' : ' '));
        }
        cout << endl;
    }

    return (int)count(painted.begin(), painted.end(), true);
}

// -------------------------------------------------------------------

int day13(const string& program)
{
    IntProc ip(program);
    ip.run();

    map<d10_pt, int> screen;
    while (ip.has_output())
    {
        auto x = ip.read_output();
        auto y = ip.read_output();
        auto t = ip.read_output();

        screen[{(int16_t)x, (int16_t)y}] = (int16_t)t;
    }

    int nblocks = 0;
    d10_pt minp((int16_t)10000, (int16_t)10000);
    d10_pt maxp((int16_t)-10000, (int16_t)-10000);
    for (const auto& tile : screen)
    {
        minp.x = min(minp.x, tile.first.x);
        minp.y = min(minp.y, tile.first.y);
        maxp.x = max(maxp.x, tile.first.x);
        maxp.y = max(maxp.y, tile.first.y);

        if (tile.second == 2)
            nblocks++;
    }

    cout << "screen: " << minp << " -> " << maxp << endl;

    return nblocks;
}

int day13_2(const string& program)
{
    IntProc ip(program);
    ip.set_input({});
    ip.poke(0, 2);

    const vector<char> tiles{ ' ','#','_','M','o' };

    size_t w = 35;
    size_t h = 25;
    vector<char> screen;
    screen.resize(w*h, tiles[0]);

    IntProc::word_t score = 0;
    IntProc::word_t ballx = 0;
    IntProc::word_t batx = 0;

    for(;;)
    {
        // update sim
        bool done = ip.run();

        // update screen
        while (ip.has_output())
        {
            auto x = ip.read_output();
            auto y = ip.read_output();
            auto t = ip.read_output();

            if (x >= 0)
                screen[x + y*w] = tiles[t];
            else
                score = t;

            if (t == 3)
                batx = x;
            else if (t == 4)
                ballx = x;
        }

        if (done)
            break;

#ifdef D_REALLY_PLAY
        // draw
        cout << "\n\nScore: " << score << '\n';
        for (size_t y = 0; y < h; ++y)
        {
            auto itscr = screen.begin() + (y * w);
            for (size_t x = 0; x < w; ++x, ++itscr)
            {
                cout << *itscr;
            }
            cout << '\n';
        }
        cout << endl;

        // read input
        auto c = _getch();
        if (c == ' ')
            ip.set_input({ 0 });
        else if (c == 'h')
            ip.set_input({ -1 });
        else if (c == 'l')
            ip.set_input({ 1 });
        else if (c == 27)
            return -1;
#else
        // autoplay
        if (batx < ballx)
            ip.set_input({ 1 });
        else if (batx > ballx)
            ip.set_input({ -1 });
        else
            ip.set_input({ 0 });
#endif
    }

    return (int)score;
}

// -------------------------------------------------------------------

void d14_tokenise(const string& in, vector<string>& out)
{
    out.clear();
    out.reserve(20);

    auto it = in.begin();
    string curr;
    while (it != in.end())
    {
        switch (*it)
        {
        case ' ':
            if (!curr.empty())
            {
                out.push_back(curr);
                curr.clear();
            }
            break;

        case ',':
            if (!curr.empty())
            {
                out.push_back(curr);
                curr.clear();
            }
            out.push_back(",");
            break;

        default:
            curr.push_back(*it);
        }

        ++it;
    }

    if(!curr.empty())
        out.push_back(curr);
}

struct d14_reagent
{
    string what;
    int64_t quant;

    d14_reagent() {/**/}
    d14_reagent(const string& squant, const string& what) : what(what), quant(stoll(squant)) {/**/ }
    d14_reagent(int64_t quant, const string& what) : what(what), quant(quant) {/**/ }
};

struct d14_reaction
{
    d14_reagent out;
    vector<d14_reagent> in;

    d14_reaction() {/**/}
    d14_reaction(const string& input)
    {
        vector<string> tokens;
        d14_tokenise(input, tokens);

        for (auto it_tok = tokens.begin(); it_tok != tokens.end(); ++it_tok)
        {
            if (*it_tok == ",")
                continue;

            if (*it_tok == "=>")
            {
                ++it_tok;
                out = d14_reagent(*it_tok, *(it_tok + 1));
                break;
            }

            in.emplace_back(*it_tok, *(it_tok + 1));
            ++it_tok;
        }
    }
};

int day14(const stringlist& input)
{
    map<string, d14_reaction> reactions;    // by output
    for (auto& line : input)
    {
        auto pos_output = line.find_last_of(' ');
        string output = line.substr(pos_output + 1);
        reactions.try_emplace(output, line);
    }

    int64_t ore_used = 0;
    list<d14_reagent> needed;
    map<string, int64_t> excess;
    needed.emplace_back(1, "FUEL");
    while (!needed.empty())
    {
        d14_reagent what = move(needed.front());
        needed.pop_front();

        // if it's ore, we need to go mining
        if (what.what == "ORE")
        {
            ore_used += what.quant;
            continue;
        }

        // do we have enough excess of that left over?
        auto itexcess = excess.find(what.what);
        if (itexcess != excess.end())
        {
            if (itexcess->second >= what.quant)
            {
                excess[what.what] = itexcess->second - what.quant;
                continue;
            }

            what.quant -= itexcess->second;
            excess[what.what] = 0;
        }

        // nope, we need to make some more
        const d14_reaction& reaction = reactions[what.what];
        int64_t numneeded = what.quant / reaction.out.quant;
        if (reaction.out.quant * numneeded < what.quant)
            numneeded++;
        if (reaction.out.quant * numneeded > what.quant)
            excess[what.what] = reaction.out.quant * numneeded - what.quant;

        for (auto& reagent : reaction.in)
            needed.emplace_back(numneeded * reagent.quant, reagent.what);
    }

    return (int)ore_used;
}

int day14_2(const stringlist& input)
{
    map<string, d14_reaction> reactions;    // by output
    for (auto& line : input)
    {
        auto pos_output = line.find_last_of(' ');
        string output = line.substr(pos_output + 1);
        reactions.try_emplace(output, line);
    }

    int64_t fuel_made = 0;
    list<d14_reagent> needed;
    map<string, int64_t> excess;
    excess["ORE"] = 1000000000000;
    for(;;)
    {
        needed.emplace_back(1, "FUEL");

        bool run_out = false;
        while (!needed.empty())
        {
            d14_reagent what = move(needed.front());
            needed.pop_front();

            // do we have enough excess of that left over?
            auto itexcess = excess.find(what.what);
            if (itexcess != excess.end())
            {
                if (itexcess->second >= what.quant)
                {
                    excess[what.what] = itexcess->second - what.quant;
                    continue;
                }

                what.quant -= itexcess->second;
                excess[what.what] = 0;
            }

            // if it's ore, we're done
            if (what.what == "ORE")
            {
                run_out = true;
                break;
            }

            // nope, we need to make some more
            const d14_reaction& reaction = reactions[what.what];
            int64_t numneeded = what.quant / reaction.out.quant;
            if (reaction.out.quant * numneeded < what.quant)
                numneeded++;
            if (reaction.out.quant * numneeded > what.quant)
                excess[what.what] = reaction.out.quant * numneeded - what.quant;

            for (auto& reagent : reaction.in)
                needed.emplace_back(numneeded * reagent.quant, reagent.what);
        }

        if (run_out)
            break;

        fuel_made++;
    }

    return (int)fuel_made;
}

// -------------------------------------------------------------------


void d16_cycle(const vector<char>& in, vector<char>& out)
{
    out.clear();
    const vector<int> base{ 0,1,0,-1 };
    for (size_t o = 1; o <= in.size(); ++o)
    {
        size_t b = 1;
        size_t bi = 0;
        if (b == o)
        {
            b = 0;
            bi = (bi + 1) & 0x3;
        }

        int el = 0;
        for (size_t i = 0; i < in.size(); ++i)
        {
            el += in[i] * base[bi];

            ++b;
            if (b == o)
            {
                b = 0;
                bi = (bi + 1) & 0x3;
            }
        }

        el = abs(el);
        out.push_back(el % 10);
    }
}

string day16(const string& input, int ncycles)
{
    vector<char> curr;
    for (auto i : input)
        curr.push_back(i - '0');

    vector<char> next;
    next.resize(input.size());

    for (int c = 0; c < ncycles; ++c)
    {
        d16_cycle(curr, next);
        next.swap(curr);
    }

    string finish;
    for (auto it = curr.begin(); it != curr.begin() + 8; ++it)
        finish.append(1, *it + '0');
    return finish;
}

string day16_2(const string& input)
{
    // Unsurprisingly, brute force is not a thing
    //  THINKS: the point our 8 char sequence starts is FAR (5,970,443 through a sequence that's 650x10,000 = 6,500,000 long)
    //  by that point, most of the high frequency churn will have been wiped out

    // coefficients for each digit are zero until the digit before them, then 1 to the end:
    //   0: sum(digits[offset-1 : 2*offset - 1]) %10
    //   1: sum(digits[offset : 2*(offset+1) - 1]) %10
    //   2: sum(digits[offset+1 : 2*(offset+2) - 1]) %10
    //  etc.

    // so the only digits contributing to our digits are [offset-(1+numcycles):end]

    // at the start, we also know that the digits repeat every 650 chars, so we can simplify to
    //   tail: floor(((len-offset)/650)) * sum(digits[0:650])
    //   0: (sum(digits[((offset-1)%650:650]) + tail) % 650
    //   1: (sum(digits[(offset)%650:650]) + tail) % 650

    // --> presumably they don't repeat every 650 chars after that though; perhaps we just run simplified logic on the last 500k digits?

    size_t numcycles = 100;
    size_t numrepeats = 10000;

    size_t wavelength = input.size();
    size_t total_len = wavelength * numrepeats;

    string off_str = input.substr(0, 7);
    size_t offset = stoul(off_str);

    vector<uint8_t> in_block;
    in_block.reserve(input.size());
    for (auto in : input)
        in_block.push_back(in - '0');

    // any digits before these cannot contribute to the final result
    size_t relevant_offset = offset - (1 + numcycles);

    size_t output_offset = 1 + numcycles;

    size_t work_len = total_len - relevant_offset;

    vector<uint8_t> curr;
    curr.reserve(work_len);
    curr.insert(curr.end(), in_block.begin() + (relevant_offset % wavelength), in_block.end());
    for (size_t i = 0; i < (work_len / wavelength); ++i)
        curr.insert(curr.end(), in_block.begin(), in_block.end());

    vector<uint8_t> next;
    next.resize(curr.size(), 0);

    for (size_t cycle = 0; cycle < numcycles; ++cycle)
    {
        cout << "cycle " << cycle << "...  " << flush;

        auto itc = curr.begin();
        auto itn = next.begin();
        // just copy the first element; it's wrong but it doesn't contribute to our final digits
        *(itn++) = *(itc++);

        // sum the appropriate window for the rest of the digits
        size_t digit_ix = relevant_offset + 1;
        for (; itn != next.end(); ++itn, ++itc, ++digit_ix)
        {
            size_t newval = accumulate(itc - 1, curr.end(), 0);
            *itn = newval % 10;
        }

        curr.swap(next);

        for (auto it = curr.begin() + output_offset; it != curr.begin() + (8 + output_offset); ++it)
            cout << (char)(*it + '0');
        cout << endl;
    }

    ostringstream os;
    for (auto it = curr.begin() + output_offset; it != curr.begin() + (output_offset + 8); ++it)
        os << (char)(*it + '0');
    return os.str();
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------

void d19_disasm(const string& program)
{
    ip_disasm(program, "19", IntProcSymbols(
    {
    },
    {
        { 282, "handle_coord_oob" },
    }));
}

int day19(const string& program)
{
    vector<int64_t> compiled;
    IntProc::parse(program, compiled);

    int64_t total = 0;
    for (int y = 0; y<50; ++y)
    {
        bool foundit = false;
        //cout << setw(3) << y << ": ";
        for (int x = 0; x < 50; ++x)
        {
            IntProc ip(compiled);
            ip.set_input({ x,y });
            ip.run();
            auto o = ip.read_output();
            total += o;
            //cout << (o ? '#' : '.');

            if (o && !foundit)
                foundit = true;
            else if (!o && foundit)
                break;
        }
        //cout << endl;
    }

    return (int)total;
}



struct d19_row
{
    int y;
    int left;
    int width;
};

int day19_2(const string& program, int santa_size = 100)
{
    vector<int64_t> compiled;
    IntProc::parse(program, compiled);

    boost::circular_buffer<d19_row> interesting_rows(santa_size);

    // skip the first bit cos it's complicated
    int top = 7;
    int left = 0;
    int right = 1;
    int interesting = 0;
    int indent = 0;
    for (int y = top; ; ++y)
    {
        bool foundit = false;

        //if (interesting > 0)
        //{
        //    cout << setw(3) << y << ": ";
        //    for (int x = indent; x < left; ++x)
        //        cout << ' ';
        //}

        for (int x = left; x < right + 20; ++x)
        {
            IntProc ip(compiled);
            ip.set_input({ x,y });
            ip.run();
            auto o = ip.read_output();

            //if(interesting)
            //    cout << (o ? '#' : '.');

            if (o && !foundit)
            {
                foundit = true;
                left = x;
                right = max(right, left + 1);

                // skip to the other end
                int skip = right - left;
                x += skip;

                //if (interesting)
                //{
                //    for (int s = 0; s < skip; ++s)
                //        cout << 's';
                //}
            }
            else if (!o && foundit)
            {
                right = x - 1;

                int width = (right - left) + 1;
                if (!interesting && width >= santa_size)
                {
                    interesting = 1;
                    indent = left - 1;
                }

                break;
            }
        }

        if (interesting)
        {
            //cout << endl;

            int width = (right - left) + 1;
            if (width < santa_size)
                interesting_rows.clear();
            else
                interesting_rows.push_back({ y, left, width });

            // if we have enough rows, we can check whether we've got our location
            if (interesting_rows.full())
            {
                const auto& toprow = interesting_rows.front();
                const auto& btmrow = interesting_rows.back();
                _ASSERT(toprow.y < btmrow.y);

                int dleft = btmrow.left - toprow.left;
                _ASSERT(dleft >= 0);

                if (toprow.width >= santa_size + dleft)
                {
                    int santax = toprow.left + dleft;
                    int santay = toprow.y;

                    return (santax * 10000) + santay;
                }
            }

            ++interesting;
        }
    }

    return -1;
}

// -------------------------------------------------------------------

void d21_disasm(const string& program)
{
    ip_disasm(program, "21", IntProcSymbols(
    {
        { 754, "total_damage" },
    },
    {
    }));
}

int ip_run_ascii(const string& program, const string& input_str, const vector<pair<int64_t, int64_t>>& pokes = {})
{
    IntProc ip(program);
    list<IntProc::word_t> input(input_str.begin(), input_str.end());
    ip.set_input(input);

    // poke any memory we want
    for (auto poke : pokes)
    {
        cout << "POKE: changing @" << poke.first << " from " << ip.peek(poke.first) << " to " << poke.second << endl;
        ip.poke(poke.first, poke.second);
    }

    ip.run();

    auto o = ip.read_final_output();

    // we made it!
    if (o > 0x7f)
        return (int)o;

    // oh dear
    string state = ip.read_output_string() + (char)o;
    size_t last_droid = 0;
    auto ix_droid = state.find_first_of("@o");
    for (;;)
    {
        cout << GREY << state.substr(last_droid, (ix_droid == string::npos) ? ix_droid : ix_droid - last_droid) << RESET;
        if (ix_droid == string::npos)
            break;
         
        cout << LIGHT_GREEN << state[ix_droid] << RESET;

        last_droid = ix_droid + 1;
        ix_droid = state.find_first_of("@o", ix_droid + 1);
    }

    cout << "  damage found so far: " << ip.peek(754) << endl;

    return -1;
}

int day21(const string& program)
{
    ostringstream input_os;
    input_os <<
        // jump if we're just about to clear a gap
        "NOT C J\n"
        "AND D J\n"
        // jump if we're about to fall into a hole
        "NOT A T\n"
        "OR T J\n"
        "WALK\n";

    return ip_run_ascii(program, move(input_os.str()), { {1503, 'o'} });
}

void d21_validate_logic()
{
    for (int x = 0; x < 512; ++x)
    {
        bool A = (x & 1) != 0;
        bool B = (x & 2) != 0;
        bool C = (x & 4) != 0;
        bool D = (x & 8) != 0;
        bool E = (x & 16) != 0;
        bool F = (x & 32) != 0;
        bool G = (x & 64) != 0;
        bool H = (x & 128) != 0;
        bool I = (x & 256) != 0;

        //    combiv2:   ~A                                   |
        //              (~B &     D & ~E &          H)        |
        //              (~B &     D &          ~G & H & ~I)   |
        //              (    ~C & D &     ~F &      H)        |
        //              (    ~C & D & ~E & F & ~G & H)        |
        //              (    ~C & D &               H & ~I)

        bool orig = !A ||
            (!B && D && !E && H) ||
            (!B && D && !G && H && !I) ||
            (!C && D && !F && H) ||
            (!C && D && !E && F && !G && H) ||
            (!C && D && H && !I);

        // ~A | (D & H & ~((B | (E & (G | I))) & (C | (F & I & (E | G)))))
        bool test = !A || (D && H && !((B || (E && (G || I))) && (C || (F && I && (E || G)))));
        _ASSERT(orig == test);

        bool T = false;
        bool J = false;

        // (C || (F && I && (E || G)))
        T |= G; // OR G T
        T |= E; // OR E T
        T &= I; // AND I T
        T &= F; // AND F T
        T |= C; // OR C T
        _ASSERT(T == ((C || (F && I && (E || G)))));

        // (B | (E & (G | I)))
        J |= I; // OR I J
        J |= G; // OR G J
        J &= E; // AND E J
        J |= B; // OR B J
        _ASSERT(J == (B || (E && (G || I))));

        // D & H & ~((B | (E & (G | I))) & (C | (F & I & (E | G)))))
        J &= T; // AND T J
        J = !J; // NOT J J
        J &= D; // AND D J
        J &= H; // AND H J
        _ASSERT(J == (D && H && !((B || (E && (G || I))) && (C || (F && I && (E || G))))));

        // ~A | ...
        T = !A; // NOT A T
        J |= T; // OR T J
        _ASSERT(orig == J);
    }
}

int day21_2(const string& program)
{
    //         0 10  1       ~B & ~E & D & H  =>  ~(B|E) & D  => ~( B | E | ~D) & H
    //        ABCDEFGHI
    // #####.##.##o#.###
    //       ^   x   v
    //        ^   %

    //      0 1   10        => ~B & D & H & ~I
    //     ABCDEFGHI
    // #####.###.##o.###
    //    ^   x   x   v
    //   ^   x   x   v
    //     ^   x   %

    //      0  0 1           ~C & ~F & H  =>    ~(C | F) & H    => ~( C | F | ~H )
    //    ABCDEFGHI
    // #####.#.o########
    //   ^   x   v
    //     ^   %    
    //    ^   %
    //  ^   %

    //      0x 0 1           ~C & ~F & H            =>    ~(C | F) & H    => ~( C | F | ~H )
    //      0x0101           ~C & ~E & F & ~G & H
    //      0x   10          ~C & H & ~I

    //                      => ~B & D & ~E & H
    //      0 x  010        => ~B & D & ~G & H & ~I
    //                          => D & ~B & H & (~E | (~G & ~I))
    //                          => D & ~B & H & (~E | ~(G | I))
    //                          => D & ~B & H & ~(E & (G | I))
    //                          => D & H & ~B & ~(E & (G | I))
    //                          => D & H & ~(B | (E & (G | I)))

    //    combiv2:   ~A                                   |
    //              (~B &     D & ~E &          H)        |
    //              (~B &     D &          ~G & H & ~I)   |
    //              (    ~C & D &     ~F &      H)        |
    //              (    ~C & D & ~E & F & ~G & H)        |
    //              (    ~C & D &               H & ~I)

    //    ...   :   ~A |
    //              (D & H &
    //               (~B &     ~E          )        |
    //               (~B &              ~G  & ~I)   |
    //               (    ~C &     ~F &    )        |
    //               (    ~C & ~E & F & ~G )        |
    //               (    ~C &              & ~I) )


    string springscript =
        // (C | (F & I & (E | G)))
         "OR G T\n"
         "OR E T\n"
        "AND I T\n"
        "AND F T\n"
         "OR C T\n"

        // (B | (E & I))
         "OR I J\n"
        "AND E J\n"
         "OR B J\n"

        // D & H & ~((B | (E & I)) & (C | (F & I & (E | G)))))
        "AND T J\n"
        "NOT J J\n"
        "AND D J\n"
        "AND H J\n"

        // ~A | ...
        "NOT A T\n"
         "OR T J\n"

        "RUN\n";

    // change our little droid from @ to o so it's cuter
    auto result = ip_run_ascii(program, springscript, { { 1503, 'o' } });
    return result;
}

// -------------------------------------------------------------------


int main()
{
    initcolours();
    srand((unsigned int)time(0));

    cout << GARLAND(2) << "  advent of code 2019  " << GARLAND(2) << endl;

#ifdef D_INTERACTIVE_15
    day15_i(LOADSTR(15));
    return 1;
#endif

    bool skiptotheend = false;
#ifdef _DEBUG
    skiptotheend = true;
#endif

    if (!skiptotheend)
    {
        test(2, day1(READ("12")));
        test(2, day1(READ("14")));
        test(654, day1(READ("1969")));
        test(33583, day1(READ("100756")));
        gogogo(day1(LOAD(1)));

        test(2, day1_2(READ("14")));
        test(966, day1_2(READ("1969")));
        test(50346, day1_2(READ("100756")));
        gogogo(day1_2(LOAD(1)));


        test<string>("2,0,0,0,99", day2("1,0,0,0,99"));
        test<string>("2,3,0,6,99", day2("2,3,0,3,99"));
        test<string>("2,4,4,5,99,9801", day2("2,4,4,5,99,0"));
        test<string>("30,1,1,4,2,5,6,0,99", day2("1,1,1,4,99,5,6,0,99"));
        test<string>("3500,9,10,70,2,3,11,0,99,30,40,50", day2("1,9,10,3,2,3,11,0,99,30,40,50"));
        gogogo<size_t>(day2Hcf(LOADSTR(2), 12, 2), 3716250);

        nononoD(day2_2(LOADSTR(2)), 6472ll);


        test<size_t>(6, day3(READ("R8,U5,L5,D3\nU7,R6,D4,L4")));
        test<size_t>(159, day3(READ("R75,D30,R83,U83,L12,D49,R71,U7,L72\nU62,R66,U55,R34,D71,R55,D58,R83")));
        test<size_t>(135, day3(READ("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\nU98,R91,D20,R16,D67,R40,U7,R15,U6,R7")));
        nononoD(day3(LOAD(3)));

        test<size_t>(30, day3_2(READ("R8,U5,L5,D3\nU7,R6,D4,L4")));
        test<size_t>(610, day3_2(READ("R75,D30,R83,U83,L12,D49,R71,U7,L72\nU62,R66,U55,R34,D71,R55,D58,R83")));
        test<size_t>(410, day3_2(READ("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\nU98,R91,D20,R16,D67,R40,U7,R15,U6,R7")));
        nononoD(day3_2(LOAD(3)));


        test<bool>(true, d4_is_code_valid(111111));
        test<bool>(false, d4_is_code_valid(223450));
        test<bool>(false, d4_is_code_valid(123789));
        gogogo(day4(193651, 649729));

        test<bool>(true, d4_is_code_valider(112233));
        test<bool>(false, d4_is_code_valider(123444));
        test<bool>(true, d4_is_code_valider(111122));
        gogogo(day4_2(193651, 649729));


        gogogo(day5(LOADSTR(5)), 7988899ll);

        test(1ll, day5_2("3,9,8,9,10,9,4,9,99,-1,8", 8));
        test(0ll, day5_2("3,9,8,9,10,9,4,9,99,-1,8", 7));
        test(1ll, day5_2("3,9,7,9,10,9,4,9,99,-1,8", 3));
        test(0ll, day5_2("3,9,7,9,10,9,4,9,99,-1,8", 10));
        test(1ll, day5_2("3,3,1108,-1,8,3,4,3,99", 8));
        test(0ll, day5_2("3,3,1108,-1,8,3,4,3,99", 10));
        test(1ll, day5_2("3,3,1107,-1,8,3,4,3,99", 1));
        test(0ll, day5_2("3,3,1107,-1,8,3,4,3,99", 10000));

        test(1ll, day5_2("3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9", 1234));
        test(0ll, day5_2("3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9", 0));
        test(1ll, day5_2("3,3,1105,-1,9,1101,0,0,12,4,12,99,1", 1234));
        test(0ll, day5_2("3,3,1105,-1,9,1101,0,0,12,4,12,99,1", 0));
        //ip_dump("3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99");
        gogogo(day5_2(LOADSTR(5), 5), 13758663ll);


        test(42, day6(LOAD(6t)));
        gogogo(day6(LOAD(6)));

        test<size_t>(4, day6_2(LOAD(6t2)));
        gogogo(day6_2(LOAD(6)));

        test(43210, day7("3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0"));
        test(54321, day7("3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0"));
        test(65210, day7("3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0"));
        gogogo(day7(LOADSTR(7)), 212460);

        test(139629729ll, day7_2("3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5"));
        test(18216ll, day7_2("3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10"));
        gogogo(day7_2(LOADSTR(7)), 21844737ll);


        test(6, day8("000000112220001122", 3, 2));
        gogogo(day8(LOADSTR(8)));

        test(1, day8_2("0222112222120000", 2, 2));
        gogogo(day8_2(LOADSTR(8)));

        const char* quine = "109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99";
        test<string>(quine, ip_run_capture_output(quine));
        test<size_t>(16, ip_run_capture_output("1102,34915192,34915192,7,4,7,99,0").size());
        test<string>("1125899906842624", ip_run_capture_output("104, 1125899906842624, 99"));
        //ip_dump(LOADSTR(9));
        gogogo<string>(ip_run_capture_output(LOADSTR(9)), "2789104029");

        gogogo<string>(ip_run_capture_output(LOADSTR(9), { 2 }));


        test<string>("8@3,4", day10(LOAD(10t)));
        test<string>("33@5,8", day10(LOAD(10t2)));
        test<string>("35@1,2", day10(LOAD(10t3)));
        nD(test<string>("41@6,3", day10(LOAD(10t4))));
        nD(test<string>("210@11,13", day10(LOAD(10t5))));
        nononoD(day10(LOAD(10)), string("340@28,29"));


        auto d10_2 = LOAD(10t_2);
        test(801, day10_2(d10_2, { 8, 3 }, 1));
        test(900, day10_2(d10_2, { 8, 3 }, 2));
        test(901, day10_2(d10_2, { 8, 3 }, 3));
        test(1000, day10_2(d10_2, { 8, 3 }, 4));
        test(203, day10_2(d10_2, { 8, 3 }, 20));
        test(800, day10_2(d10_2, { 8, 3 }, 31));
        gogogo(day10_2(LOAD(10), { 28, 29 }, 200), 2628);


        // ip_dump(LOADSTR(11), { 0, 471 });
        gogogo(day11(LOADSTR(11)));
        gogogo(day11(LOADSTR(11), true));


        test(179, day12({ { -1,0,2 },{ 2,-10,-7 },{ 4,-8,8 },{ 3,5,-1 } }, 10));
        test(1940, day12({ { -8,-10,0 },{ 5,5,10 },{ 2,-7,3 },{ 9,-8,-3 } }, 100));
        gogogo(day12({ {13,9,5}, {8,14,-2}, {-5,4,11}, {2,-6,1} }, 1000));

        test(2772ll, day12_2b({ { -1,0,2 },{ 2,-10,-7 },{ 4,-8,8 },{ 3,5,-1 } }));
        test(4686774924ll, day12_2b({ { -8,-10,0 },{ 5,5,10 },{ 2,-7,3 },{ 9,-8,-3 } }));
        gogogo(day12_2b({ { 13,9,5 },{ 8,14,-2 },{ -5,4,11 },{ 2,-6,1 } }), 277068010964808ll);


        gogogo(day13(LOADSTR(13)), 216);
        nononoD(day13_2(LOADSTR(13)), 10025);


        test(31, day14(LOAD(14t)));
        test(165, day14(LOAD(14t2)));
        test(2210736, day14(LOAD(14t3)));
        nononoD(day14(LOAD(14)));

        nest(460664, day14_2(LOAD(14t3)));
        nonono(day14_2(LOAD(14)));


        gogogo(day15(LOADSTR(15)));
        gogogo(day15_2(LOADSTR(15)));


        test<string>("48226158", day16("12345678", 1));
        test<string>("34040438", day16("12345678", 2));
        test<string>("03415518", day16("12345678", 3));
        test<string>("01029498", day16("12345678", 4));
        test<string>("24176176", day16("80871224585914546619083218645595", 100));
        test<string>("73745418", day16("19617804207202209144916044189917", 100));
        test<string>("52432133", day16("69317163492948606335995924319873", 100));
        nononoD(day16(LOADSTR(16), 100));

        // TODO: return to 16 pt2!
        nest(string("84462026"), day16_2("03036732577212944063491565474664"));
        nest(string("78725270"), day16_2("02935109699940807407585447034323"));
        nest(string("53553731"), day16_2("03081770884921959731165446850517"));
        nonono(day16_2(LOADSTR(16)));


        test(76, d17_sum_alignments(LOAD(17t)));
        gogogo(day17(LOADSTR(17)));

        gogogo(day17_2(LOADSTR(17)));


        test(8, day18(LOAD(18t)));
        test(86, day18(LOAD(18t2)));
        test(132, day18(LOAD(18t3)));
        nD(test(136, day18(LOAD(18t4))));
        nononoD(day18(LOAD(18)));

        // bollocks to part 2...
        nononoD(day18(LOAD(18)));


        gogogo(day19(LOADSTR(19)), 166);
        nononoD(day19_2(LOADSTR(19)));


        test(23, day20(LOAD(20t)));
        test(58, day20(LOAD(20t2)));
        gogogo(day20(LOAD(20)));

        test(26, day20_2(LOAD(20t)));
        test(396, day20_2(LOAD(20t3)));
        //test(-1, day20_2(LOAD(20t2)));    // docs say there is no path, but this code finds (a very long) one - 3,866,354 steps
        nononoD(day20_2(LOAD(20)));
    }
    else
    {
        gday = 21;
    }


    gogogo(day21(LOADSTR(21)));
    gogogo(day21_2(LOADSTR(21)));


    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

