#include "harness.h"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <unordered_set>
#include <vector>

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

void ip_dump(const string& initial, const list<IntProc::word_t>& entries = { 0 })
{
    IntProc ip(initial);
    ip.dump(entries);
}

// -------------------------------------------------------------------

struct d6_planet
{
    string parent;
    vector<string> children;

    d6_planet() {/**/}
    d6_planet(const string& parent, vector<string>& children) : parent(parent), children(children){/**/}
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
    size_t shared = 0;
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
int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

struct d10_pt
{
    int16_t x, y;

    d10_pt() {/**/}
    d10_pt(int16_t x, int16_t y) : x(x), y(y) {/**/}
    d10_pt(initializer_list<int> l) {
        auto it = l.begin();
        x = *(it++);
        y = *it;
    }

    bool operator==(const d10_pt& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const d10_pt& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }
    bool operator<(const d10_pt& rhs) const
    {
        if (x < rhs.x)
            return true;
        if (x > rhs.x)
            return false;
        if (y < rhs.y)
            return true;
        return false;
    }
    d10_pt& operator+=(const d10_pt& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    d10_pt operator+(const d10_pt& rhs) const
    {
        return d10_pt(x + rhs.x, y + rhs.y);
    }
    d10_pt operator-(const d10_pt& rhs) const
    {
        return d10_pt(x - rhs.x, y - rhs.y);
    }

    void simplify()
    {
        int div = abs(gcd(x, y));
        x /= div;
        y /= div;
    }
};

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

struct d12_moon
{
    d12_pt pos;
    d12_pt vel;

    d12_moon(const d12_pt& pos) : pos(pos), vel({ 0,0,0 })  {/**/}


    int energy() const
    {
        int pe = abs(pos.x) + abs(pos.y) + abs(pos.z);
        int ke = abs(vel.x) + abs(vel.y) + abs(vel.z);
        return pe * ke;
    }

    void update_velocity(const vector<d12_moon>& moons)
    {
        for (auto it = moons.begin(); it!= moons.end(); ++it)
        {
            const d12_moon& o = *it;
            if (&o == this)
                continue;

            vel.x += (o.pos.x > pos.x) ? 1 : ((o.pos.x < pos.x) ? -1 : 0);
            vel.y += (o.pos.y > pos.y) ? 1 : ((o.pos.y < pos.y) ? -1 : 0);
            vel.z += (o.pos.z > pos.z) ? 1 : ((o.pos.z < pos.z) ? -1 : 0);
        }
    }

    void update_position()
    {
        pos += vel;
    }
};
ostream& operator<<(ostream& os, const d12_moon& m)
{
    os << "pos=" << m.pos << ", vel=" << m.vel;
    return os;
}


int day12(const vector<d12_pt>& input, int steps, bool show = false)
{
    vector<d12_moon> moons;
    for (auto& p : input)
        moons.emplace_back(p);

    for (int tick = 1; tick <= steps; ++tick)
    {
        for (auto& moon : moons)
            moon.update_velocity(moons);

        for (auto& moon : moons)
            moon.update_position();

        if (show)
        {
            cout << "After " << tick << " step" << (tick == 1 ? "" : "s") << ":\n";
            for (const auto& moon : moons)
                cout << "   " << moon << '\n';
        }
    }

    int energy = 0;
    for (auto& moon : moons)
    {
        if (show)
            cout << "  energy = " << moon.energy() << "\n";
        energy += moon.energy();
    }
    return energy;
}

// -------------------------------------------------------------------


int main()
{
    initcolours();
    srand((unsigned int)time(0));

    cout << GARLAND(2) << "  advent of code 2019  " << GARLAND(2) << endl;


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


    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

