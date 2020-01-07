#include <map>
#include <set>
#include <vector>

#include "harness.h"


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
        case 'D':   dy = -1;     break;
        case 'R':   dx = 1;     break;
        case 'L':   dx = -1;     break;
        default:    throw "I can't go that way o_O";
        }
        int count = atoi(step.c_str() + 1);

        for (; count > 0; --count)
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
