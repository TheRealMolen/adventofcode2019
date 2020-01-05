#include "harness.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <tmmintrin.h>
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

// -------------------------------------------------------------------

// fuel counter upper
inline int fuel_for_mass(int mass) {
    return max((mass / 3) - 2, 0);
}
inline int total_fuel_for_mass(int mass) {
    int total = 0;
    int lastmass = mass;
    int extra_fuel;
    do {
        extra_fuel = fuel_for_mass(lastmass);
        total += extra_fuel;
        lastmass = extra_fuel;
    } while (extra_fuel > 0);
    return total;
}

int day1(const stringlist& module_sizes)
{
    int total = 0;
    for (const auto& module_size : module_sizes)
    {
        int size = stoi(module_size);
        int fuel = fuel_for_mass(size);
        total += fuel;
    }
    return total;
}
int day1_2(const stringlist& module_sizes)
{
    int total = 0;
    for (const auto& module_size : module_sizes)
    {
        int size = stoi(module_size);
        int fuel = total_fuel_for_mass(size);
        total += fuel;
    }
    return total;
}

// -------------------------------------------------------------------

string day2(const string& initial)
{
    IntProc ip(initial);
    ip.run();

    ostringstream oss;
    oss << ip;
    return oss.str();
}


size_t day2Hcf(const string& initial, IntProc::word_t noun, IntProc::word_t verb)
{
    IntProc ip(initial);
    ip.poke(1, noun);
    ip.poke(2, verb);

    ip.run();

    return ip.peek(0);
}

IntProc::word_t day2_2(const string& initial)
{
    for (IntProc::word_t verb = 0; verb < 100; verb++)
    {
        for (IntProc::word_t noun = 0; noun < 100; noun++)
        {
            if (day2Hcf(initial, noun, verb) == 19690720)
            {
                return noun * 100 + verb;
            }
        }
    }
    return 999999999;
}


// -------------------------------------------------------------------

union d3_u_point
{
    uint64_t u;
    struct
    {
        int32_t x, y;
    } p;
};

void d3_trace_wire(const string& in_def, vector<uint64_t>& out_points)
{
    out_points.clear();

    string def(in_def);
    size_t start = 0;
    size_t sep;
    d3_u_point p;
    p.u = 0;

    do {
        sep = def.find(',', start);
        string step = def.substr(start, sep != string::npos ? sep - start : sep);
        start = sep + 1;

        int dx = 0, dy = 0;
        char dir = step[0];
        switch (dir)
        {
        case 'U':   dy = 1;     break;
        case 'D':   dy =-1;     break;
        case 'R':   dx = 1;     break;
        case 'L':   dx =-1;     break;
        default:    throw "I can't go that way o_O";
        }
        int count = atoi(step.c_str() + 1);

        for ( ; count > 0; --count)
        {
            p.p.x += dx;
            p.p.y += dy;
            out_points.push_back(p.u);
        }

    } while (sep != string::npos);
}

size_t day3(const stringlist& wires)
{
    vector<uint64_t> wire1, wire2;
    auto it_wire = wires.begin();
    d3_trace_wire(*(it_wire++), wire1);
    d3_trace_wire(*(it_wire++), wire2);

    set<uint64_t> w2_set(wire2.begin(), wire2.end());

    size_t closest = ~0u;
    for (auto p1 : wire1)
    {
        if (w2_set.find(p1) != w2_set.end())
        {
            d3_u_point p;
            p.u = p1;
            size_t dist = abs(p.p.x) + abs(p.p.y);
            if (dist < closest)
            {
                closest = dist;
            }
        }
    }

    return closest;
}

size_t day3_2(const stringlist& wires)
{
    vector<uint64_t> wire1, wire2;
    auto it_wire = wires.begin();
    d3_trace_wire(*(it_wire++), wire1);
    d3_trace_wire(*(it_wire++), wire2);

    map<uint64_t, size_t> w2_lengths;
    for (size_t i = 0; i < wire2.size(); ++i)
        w2_lengths.insert(make_pair(wire2[i], i + 1));

    size_t shortest = ~0u;
    for (auto it1 = wire1.begin(); it1 != wire1.end(); ++it1)
    {
        auto p1 = *it1;

        auto it2 = w2_lengths.find(p1);
        if (it2 != w2_lengths.end())
        {
            size_t dist = 1 + distance(wire1.begin(), it1) + it2->second;
            if (dist < shortest)
            {
                shortest = dist;
            }
        }
    }

    return shortest;
}


// -------------------------------------------------------------------

const int D4_DIGITS = 6;
inline bool d4_is_code_valid(int code)
{
    int digits[D4_DIGITS];
    for (int i = 0; i < D4_DIGITS; ++i)
    {
        digits[(D4_DIGITS - 1) - i] = code % 10;
        code /= 10;
    }

    for (int i = 0; i < D4_DIGITS - 1; ++i)
    {
        if (digits[i + 1] < digits[i])
            return false;
    }

    bool found_dupe = false;
    for (int i = 0; i < D4_DIGITS - 1; ++i)
    {
        if (digits[i + 1] == digits[i])
        {
            found_dupe = true;
            break;
        }
    }

    return found_dupe;
}
inline bool d4_is_code_valider(int code)
{
    int digits[D4_DIGITS];
    for (int i = 0; i < D4_DIGITS; ++i)
    {
        digits[(D4_DIGITS - 1) - i] = code % 10;
        code /= 10;
    }

    for (int i = 0; i < D4_DIGITS - 1; ++i)
    {
        if (digits[i + 1] < digits[i])
            return false;
    }

    bool found_dupe = false;
    for (int i = 0; i < D4_DIGITS - 1; ++i)
    {
        if (digits[i + 1] == digits[i])
        {
            if (i == D4_DIGITS - 2 || digits[i + 2] != digits[i])
            {
                found_dupe = true;
                break;
            }
            else
            {
                // skip the whole run
                auto run = digits[i];
                do {
                    i++;
                } while (i < D4_DIGITS - 2 && digits[i+1] == run);
            }
        }
    }

    return found_dupe;
}

int day4(int lo, int hi)
{
    int total = 0;

    for (int c = lo; c < hi; ++c)
    {
        if (d4_is_code_valid(c))
        {
            total++;
        }
    }

    return total;
}

int day4_2(int lo, int hi)
{
    int total = 0;

    for (int c = lo; c < hi; ++c)
    {
        if (d4_is_code_valider(c))
        {
            total++;
        }
    }

    return total;
}


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

void ip_dump(const string& initial, const IntProcSymbols& syms)
{
    IntProc ip(initial);
    ip.dump(syms);
}

void ip_disasm(const string& program, const string& filename, const IntProcSymbols& syms)
{
    string filepath = "data/day" + filename + ".s";
    ofstream ofs(filepath);

    ofs << "# IntProc disassembly\n#\n\n";

    IntProc ip(program);
    ip.dump(ofs, syms);

    ofs.close();
}

// -------------------------------------------------------------------

struct d6_planet
{
    string parent;
    vector<string> children;

    d6_planet() {/**/}
    d6_planet(const string& parent, const vector<string>& children) : parent(parent), children(children){/**/}
};

int d6_count_orbits(const string& start, const map<string, d6_planet>& subtrees, int depth=0)
{
    auto it = subtrees.find(start);
    if (it == subtrees.end())
    {
        throw "couldn't find child...";
    }

    // the total starts with our depth
    int total = depth;

    // then we add on the totals for all our children
    for (auto& childname : it->second.children)
    {
        total += d6_count_orbits(childname, subtrees, depth + 1);
    }

    return total;
}

void d6_parse_subtrees(const stringlist& input, map<string, d6_planet>& out_subtrees)
{
    // capture all the parent->children relationships in a flat structure
    // this is because the input data can be in any order
    for (auto line : input)
    {
        auto sep = line.find(')');
        if (sep == string::npos)
            continue;

        string centre = line.substr(0, sep);
        string orbiter = line.substr(sep + 1);

        // ensure we have a record for centre that includes orbiter
        auto it_centre = out_subtrees.find(centre);
        if (it_centre == out_subtrees.end())
        {
            out_subtrees.try_emplace(centre, "", vector<string>{ orbiter });
        }
        else
        {
            it_centre->second.children.emplace_back(orbiter);
        }

        // ensure we have a record for orbiter
        auto it_orbiter = out_subtrees.find(orbiter);
        if (it_orbiter == out_subtrees.end())
        {
            out_subtrees.emplace(orbiter, d6_planet( centre, vector<string>{}));
        }
        else
        {
            it_orbiter->second.parent = centre;
        }
    }
}

void d6_find_path(const string& start, const string& end, const map<string, d6_planet>& subtrees, vector<string>& out_path)
{
    out_path.clear();

    auto it = subtrees.find(end);
    while (it->first != start)
    {
        out_path.emplace_back(it->first);
        it = subtrees.find(it->second.parent);
    }
    out_path.emplace_back(it->first);
    reverse(out_path.begin(), out_path.end());
}

size_t d6_count_transfers(const string& start, const string& end, const map<string, d6_planet>& subtrees)
{
    vector<string> path_to_start;
    vector<string> path_to_end;
    d6_find_path("COM", start, subtrees, path_to_start);
    d6_find_path("COM", end, subtrees, path_to_end);

    path_to_start.pop_back();
    path_to_end.pop_back();
    auto it_s = path_to_start.begin();
    auto it_e = path_to_end.begin();
    while(*it_s == *it_e && it_s != path_to_start.end() && it_e != path_to_end.end())
    {
        ++it_s;
        ++it_e;
    }

    return distance(it_s, path_to_start.end()) + distance(it_e, path_to_end.end());
}


int day6(const stringlist& input)
{
    map<string, d6_planet> subtrees;
    d6_parse_subtrees(input, subtrees);

    return d6_count_orbits("COM", subtrees);
}

size_t day6_2(const stringlist& input)
{
    map<string, d6_planet> subtrees;
    d6_parse_subtrees(input, subtrees);

    return d6_count_transfers("YOU", "SAN", subtrees);
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
template<typename T>
T gcd(T a, T b)
{
    while (b != 0)
    {
        T t = b;
        b = a % b;
        a = t;
    }
    return a;
}

template<typename T>
struct pt2d
{
    T x, y;

    pt2d() {/**/}
    pt2d(T x, T y) : x(x), y(y) {/**/}
    pt2d(initializer_list<T> l) {
        auto it = l.begin();
        x = *(it++);
        y = *it;
    }

    bool operator==(const pt2d<T>& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const pt2d<T>& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }
    bool operator<(const pt2d<T>& rhs) const
    {
        if (x < rhs.x)
            return true;
        if (x > rhs.x)
            return false;
        if (y < rhs.y)
            return true;
        return false;
    }
    pt2d<T>& operator+=(const pt2d<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    pt2d<T> operator+(const pt2d<T>& rhs) const
    {
        return pt2d<T>(x + rhs.x, y + rhs.y);
    }
    pt2d<T> operator-(const pt2d<T>& rhs) const
    {
        return pt2d<T>(x - rhs.x, y - rhs.y);
    }

    void simplify()
    {
        T div = (T)abs(gcd(x, y));
        x /= div;
        y /= div;
    }
};
template<typename T>
ostream& operator<<(ostream& os, const pt2d<T>& p)
{
    os << '<' << p.x << ", " << p.x << '>';
    return os;
}

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
template<typename T>
T gcd(T a, T b, T c)
{
    return max(gcd(a, b), max(gcd(b, c), gcd(c, a)));
}

template<typename T>
T lcm(T a, T b)
{
    return (a * b) / gcd(a, b);
}


struct d12_pt
{
    int x, y, z;

    d12_pt(const initializer_list<int>& l)
    {
        auto it = l.begin();
        x = *(it++);
        y = *(it++);
        z = *it;
    }
    d12_pt(int x, int y, int z) : x(x), y(y), z(z) {/**/}

    d12_pt& operator+=(const d12_pt& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};
ostream& operator<<(ostream& os, const d12_pt& v)
{
    os << "<x=" << setw(2) << setfill(' ') << v.x << ", y=" << setw(2) << v.y << ", z=" << setw(2) << v.z << '>';
    return os;
};

struct d12_moon4
{
    __m128i px, py, pz;
    __m128i vx, vy, vz;

    d12_moon4(const vector<d12_pt>& i)
    {
        px = _mm_set_epi32(i[0].x, i[1].x, i[2].x, i[3].x);
        py = _mm_set_epi32(i[0].y, i[1].y, i[2].y, i[3].y);
        pz = _mm_set_epi32(i[0].z, i[1].z, i[2].z, i[3].z);
        vx = _mm_setzero_si128();
        vy = _mm_setzero_si128();
        vz = _mm_setzero_si128();
    }

    bool operator==(const d12_moon4& o) const
    {
        __m128i pxe = _mm_cmpeq_epi32(px, o.px);
        __m128i pye = _mm_cmpeq_epi32(py, o.py);
        __m128i pze = _mm_cmpeq_epi32(pz, o.pz);
        __m128i vxe = _mm_cmpeq_epi32(vx, o.vx);
        __m128i vye = _mm_cmpeq_epi32(vy, o.vy);
        __m128i vze = _mm_cmpeq_epi32(vz, o.vz);

        __m128i ceq = _mm_and_si128(_mm_and_si128(_mm_and_si128(pxe, pye), _mm_and_si128(vxe, vye)), _mm_and_si128(pze, vze));

        return _mm_test_all_ones(ceq) != 0;
    }

    __forceinline __m128i update_vel_cpt(__m128i c)
    {
        __m128i m1 = _mm_shuffle_epi32(c, 0x39);
        __m128i m2 = _mm_shuffle_epi32(c, 0x4e);
        __m128i m3 = _mm_shuffle_epi32(c, 0x93);

        __m128i d1 = _mm_sub_epi32(m1, c);
        __m128i d2 = _mm_sub_epi32(m2, c);
        __m128i d3 = _mm_sub_epi32(m3, c);

        __m128i one = _mm_set1_epi32(1);

        __m128i dv1 = _mm_sign_epi32(one, d1);
        __m128i dv2 = _mm_sign_epi32(one, d2);
        __m128i dv3 = _mm_sign_epi32(one, d3);

        return _mm_add_epi32(dv1, _mm_add_epi32(dv2, dv3));
    }

    void update()
    {
        vx = _mm_add_epi32(vx, update_vel_cpt(px));
        vy = _mm_add_epi32(vy, update_vel_cpt(py));
        vz = _mm_add_epi32(vz, update_vel_cpt(pz));

        px = _mm_add_epi32(px, vx);
        py = _mm_add_epi32(py, vy);
        pz = _mm_add_epi32(pz, vz);
    }

    int energy() const
    {
        __m128i pe = _mm_add_epi32(_mm_abs_epi32(px), _mm_add_epi32(_mm_abs_epi32(py), _mm_abs_epi32(pz)));
        __m128i ke = _mm_add_epi32(_mm_abs_epi32(vx), _mm_add_epi32(_mm_abs_epi32(vy), _mm_abs_epi32(vz)));
        __m128i te = _mm_mullo_epi32(pe, ke);
        __m128i hsum = _mm_hadd_epi32(te, te);
        __m128i sum = _mm_hadd_epi32(hsum, hsum);
        return sum.m128i_i32[0];
    }



    void updateX()
    {
        vx = _mm_add_epi32(vx, update_vel_cpt(px));
        px = _mm_add_epi32(px, vx);
    }
    bool eqX(const d12_moon4& o) const
    {
        __m128i peq = _mm_cmpeq_epi32(px, o.px);
        __m128i veq = _mm_cmpeq_epi32(vx, o.vx);
        __m128i ceq = _mm_and_si128(peq, veq);
        return _mm_test_all_ones(ceq) != 0;
    }

    void updateY()
    {
        vy = _mm_add_epi32(vy, update_vel_cpt(py));
        py = _mm_add_epi32(py, vy);
    }
    bool eqY(const d12_moon4& o) const
    {
        __m128i peq = _mm_cmpeq_epi32(py, o.py);
        __m128i veq = _mm_cmpeq_epi32(vy, o.vy);
        __m128i ceq = _mm_and_si128(peq, veq);
        return _mm_test_all_ones(ceq) != 0;
    }

    void updateZ()
    {
        vz = _mm_add_epi32(vz, update_vel_cpt(pz));
        pz = _mm_add_epi32(pz, vz);
    }
    bool eqZ(const d12_moon4& o) const
    {
        __m128i peq = _mm_cmpeq_epi32(pz, o.pz);
        __m128i veq = _mm_cmpeq_epi32(vz, o.vz);
        __m128i ceq = _mm_and_si128(peq, veq);
        return _mm_test_all_ones(ceq) != 0;
    }
};

ostream& operator<<(ostream& os, const d12_moon4& m)
{
    for (int i = 3; i >= 0; --i)
    {
        os << setfill(' ')
            << "   pos=<x=" << setw(2) << m.px.m128i_i32[i]
            << ", y=" << setw(2) << m.py.m128i_i32[i]
            << ", z=" << setw(2) << m.pz.m128i_i32[i]
            << ">, vel=<x=" << setw(2) << m.vx.m128i_i32[i]
            << ", y=" << setw(2) << m.vy.m128i_i32[i]
            << ", z=" << setw(2) << m.vz.m128i_i32[i] << ">" << endl;
    }
    return os;
}


int day12(const vector<d12_pt>& input, int steps, bool show = false)
{
    d12_moon4 moons(input);

    const int64_t chunk = 100000000;
    int64_t taken = 1;
    while ((steps - taken) > chunk)
    {
        for (int64_t tick = 1; tick <= chunk; ++tick)
        {
            moons.update();
        }
        taken += chunk;
        cout << "After " << taken << " steps:\n" << moons;
    }

    for (int64_t tick = taken; tick <= steps; ++tick)
    {
        moons.update();
    }

    if (show)
    {
        cout << "After " << steps << " steps, energy=" << moons.energy() << "; moons=\n" << moons;
    }

    return moons.energy();
}


// bruteforce (>100bn steps and counting)
int64_t day12_2(const vector<d12_pt>& input, bool show = false)
{
    d12_moon4 moons(input);
    vector<d12_moon4> zeros{ input };

    int64_t taken = 0;
    for (;;)
    {
        moons.update();
        taken++;

        if (moons.energy() == 0)
        {
            if (find(zeros.begin(), zeros.end(), moons) != zeros.end())
                break;

            zeros.push_back(moons);
            cout << "found zero after " << taken << " steps:\n" << moons << endl;
        }
    }

    if (show)
    {
        cout << "After " << taken << " steps, energy=" << moons.energy() << "; moons=\n" << moons << endl;
    }

    return taken;
}


int64_t day12_2b(const vector<d12_pt>& input, bool show = false)
{
    d12_moon4 moons(input);
    d12_moon4 original(input);

    // find loop length for x
    int64_t takenX = 0;
    for (;;)
    {
        moons.updateX();
        takenX++;

        if (moons.eqX(original))
        {
            if (show)
                cout << "found X loop after " << takenX << " steps" << endl;
            break;
        }
    }
    // find loop length for y
    int64_t takenY = 0;
    for (;;)
    {
        moons.updateY();
        takenY++;

        if (moons.eqY(original))
        {
            if (show)
                cout << "found Y loop after " << takenY << " steps" << endl;
            break;
        }
    }
    // find loop length for z
    int64_t takenZ = 0;
    for (;;)
    {
        moons.updateZ();
        takenZ++;

        if (moons.eqZ(original))
        {
            if(show)
                cout << "found Z loop after " << takenZ << " steps" << endl;
            break;
        }
    }

    return lcm(takenX, lcm(takenY, takenZ));
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
    d10_pt pos;
    vector<int8_t> tiles;   // -1 = untested, 0 = wall, 1 = clear, 2 = console, 3 = start
    vector<uint16_t> costs;

    d15_map(size_t size) : width(size), height(size), pos((int16_t)size/2, (int16_t)size/2)
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

    void set(const d10_pt& p, int8_t val)
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

static const vector<d10_pt> d15_moves = { {0,0}, {0,-1}, {0,1}, {-1,0}, {1,0} };

struct d15_node
{
    d10_pt pos;
    vector<int8_t> open;
    int8_t return_move;

    void try_add_move(int8_t move, const d15_map& map)
    {
        auto npos = pos + d15_moves[move];
        if (map.get(npos.x, npos.y) == -1)
            open.push_back(move);
    }

    d15_node(d10_pt _pos, int8_t move_here, const d15_map& map) : pos(_pos)
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

    for(;;)
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
    for (size_t y = 1; y < m.height-1; ++y)
    {
        auto it = m.tiles.begin() + (y * m.width) + 1;
        for (size_t x = 1; x < m.width-1; ++x, ++it)
        {
            if ((*it != '#') ||
                (*(it - 1) != '#') ||
                (*(it + 1) != '#') ||
                (*(it - m.width) != '#') ||
                (*(it + m.width) != '#'))
                continue;

            size_t alignment = (x-1) * (y-1);   // -1 for padding
            total += alignment;
        }
    }

    return (int)total;
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
static const vector<d17_pt> d17_dirs{ {0,-1}, {1,0}, {0,1}, {-1,0} };

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

        out.replace(it, it+nlen, replacement);
    }
}

struct d17_program
{
    string master;
    string a, b, c;
    bool clean;

    d17_program() : clean(false) {/**/}


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
    input_os << route.master << '\n' << route.a << '\n' << route.b << '\n' << route.c << '\n' << (video_feed?'y':'n') << '\n';
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


// -------------------------------------------------------------------
typedef pt2d<int16_t> d18_pt;
static const vector<d18_pt> d18_dirs = { {0,-1},{1,0},{0,1},{-1,0} };

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
    }
    else
    {
        gday = 18;
    }

    test(132, day18(LOAD(18t3)));
    test(86, day18(LOAD(18t2)));
    test(8, day18(LOAD(18t)));
    nD(test(136, day18(LOAD(18t4))));
    nononoD(day18(LOAD(18)));

    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

