#include "harness.h"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "intproc.h"



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
    ip.store(1, noun);
    ip.store(2, verb);

    ip.run();

    return ip.fetch(0);
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
    return ip.last_output;
}

IntProc::word_t day5_2(const string& initial, IntProc::word_t input)
{
    IntProc ip(initial);
    ip.set_input({ input });
    ip.run();
    return ip.last_output;
}

void ip_dump(const string& initial)
{
    IntProc ip(initial);
    ip.dump();
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
    return (int)ip.last_output;
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
            output = amp.last_output;
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


    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

