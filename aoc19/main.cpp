﻿#include "harness.h"

#include <algorithm>
#include <ctime>
#include <map>
#include <set>
#include <vector>



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

class IntProc
{
    size_t pc;
    vector<size_t> mem;

    friend ostream& operator<<(ostream& os, const IntProc& ip);
    
public:
    IntProc(const string& initial) : pc(0)
    {
        string in(initial);
        size_t start = 0;
        size_t sep;
        
        do {
            sep = in.find(',', start);
            if (sep != string::npos)
            {
                in[sep] = '\0';
            }

            int i = atoi(in.c_str() + start);
            if (i < 0)
            {
                throw "Invalid initial state: got a negative";
            }

            mem.push_back((size_t)i);
            start = sep + 1;
        } while (sep != string::npos);
    }

    size_t fetch(size_t addr) const
    {
        if (addr >= mem.size())
        {
            throw "IntProc:ADDR_VIOLATION";
        }
        return mem[addr];
    }

    void store(size_t addr, size_t val)
    {
        if (addr >= mem.size())
        {
            throw "IntProc:ADDR_VIOLATION";
        }
        mem[addr] = val;
    }

    bool run()
    {
        for (;;)
        {
            auto instr = fetch(pc);
            switch (instr)
            {
            case 1:
            {
                auto opA = fetch(fetch(pc + 1));
                auto opB = fetch(fetch(pc + 2));
                auto addrOut = fetch(pc + 3);
                store(addrOut, opA + opB);
                pc += 4;
                break;
            }

            case 2:
            {
                auto opA = fetch(fetch(pc + 1));
                auto opB = fetch(fetch(pc + 2));
                auto addrOut = fetch(pc + 3);
                store(addrOut, opA * opB);
                pc += 4;
                break;
            }

            case 99:
                return true;

            default:
                cerr << "IntProc got a wonky instruction " << instr << "@" << pc << "\nfinal state: " << *this << endl;
                return false;
            }
        }
    }
};

ostream& operator<<(ostream& os, const IntProc& ip)
{
    for (auto it = ip.mem.begin(); it != ip.mem.end(); ++it)
    {
        if (it != ip.mem.begin())
        {
            os << ',';
        }
        os << *it;
    }

    return os;
}


string day2(const string& initial)
{
    IntProc ip(initial);
    ip.run();

    ostringstream oss;
    oss << ip;
    return oss.str();
}


size_t day2Hcf(const string& initial, size_t noun, size_t verb)
{
    IntProc ip(initial);
    ip.store(1, noun);
    ip.store(2, verb);

    ip.run();

    return ip.fetch(0);
}

size_t day2_2(const string& initial)
{
    for (size_t verb = 0; verb < 100; verb++)
    {
        for (size_t noun = 0; noun < 100; noun++)
        {
            if (day2Hcf(initial, noun, verb) == 19690720)
            {
                return noun * 100 + verb;
            }
        }
    }
    return 999999999u;
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
    gogogo(day2Hcf(LOADSTR(2), 12, 2));

    nononoD(day2_2(LOADSTR(2)));


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


    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

