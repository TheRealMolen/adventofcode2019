#include <map>
#include <vector>

#include "harness.h"



struct d6_planet
{
    string parent;
    vector<string> children;

    d6_planet() {/**/ }
    d6_planet(const string& parent, const vector<string>& children) : parent(parent), children(children) {/**/ }
};

int d6_count_orbits(const string& start, const map<string, d6_planet>& subtrees, int depth = 0)
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
            out_subtrees.emplace(orbiter, d6_planet(centre, vector<string>{}));
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
    while (*it_s == *it_e && it_s != path_to_start.end() && it_e != path_to_end.end())
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
