//
// clearly i need to learn more about bfs (&dijkstra!)
//

// this example based on:
//   https://leetcode.com/problems/minimum-genetic-mutation/description/
// and
//   https://www.freecodecamp.org/news/deep-dive-into-graph-traversals-227a90c6a261/

#include <map>
#include <queue>
#include <vector>

#include "../aoc19/harness.h"


bool is_valid_mutation(const string& from, const string& to)
{
    if (from.size() != to.size())
        return false;

    bool same = true;
    for (size_t i = 0; i < from.size(); ++i)
    {
        if (from[i] != to[i])
        {
            if (same)
                same = false;
            else
                return false;
        }
    }
    return true;
}

struct node
{
    bool discovered;
    vector<string> connections;

    string val;
    string parent;

    node() {/**/}
    node(const string& s) : discovered(false), val(s) {/**/}
};

int count_mutations(const string& start, const string& end, const stringlist& bank)
{
    if (start == end)
        return 0;

    map<string, node> graph;

    // stuff everything into our graph
    for (auto& seq : bank)
        graph.emplace(seq, seq);
    if (graph.find(start) == graph.end())
        graph.emplace(start, start);
    if (graph.find(end) == graph.end())
        graph.emplace(end, end);

    // now join all of our nodes
    for (auto it_node = graph.begin(); it_node != graph.end(); ++it_node)
    {
        auto it_test = it_node;
        ++it_test;
        for (; it_test != graph.end(); ++it_test)
        {
            if (is_valid_mutation(it_node->first, it_test->first))
            {
                it_node->second.connections.push_back(it_test->first);
                it_test->second.connections.push_back(it_node->first);
            }
        }
    }

    // do a bfs (note: our nodes all start off as "undiscovered")
    queue<const string*> q;
    q.push(&start);
    graph[start].discovered = true;
    bool done = false;
    while (!done && !q.empty())
    {
        const string& curr = *q.front();
        q.pop();

        // add all undiscovered connections to our to-do list
        const auto& curr_node = graph.at(curr);
        for (const string& next : curr_node.connections)
        {
            auto& next_node = graph.at(next);
            if (!next_node.discovered)
            {
                next_node.parent = curr;
                next_node.discovered = true;

                // are we done?
                if (next == end)
                {
                    done = true;
                    break;
                }

                q.push(&next);
            }
        }
    }

    if (!done)
    {
        cout << "couldn't find a valid path from " << start << " to " << end << endl;
        return -1;
    }

    // find the path, backwards
    vector<string> path;
    for (const node* pcurr = &graph.at(end);;)
    {
        path.push_back(pcurr->val);

        if (pcurr->parent.empty())
            break;
        pcurr = &graph.at(pcurr->parent);
    }
    reverse(path.begin(), path.end());

    cout << "...found path: ";
    for (auto& n : path)
    {
        if (n != path.front())
            cout << " -> ";
        cout << n;
    }
    cout << endl;
    
    return (int)(path.size() - 1);
}



int main()
{
    cout << "mutationist...\n" << endl;

    cout << "> test0: " << count_mutations("AACCGGTT", "AACCGGTA", stringlist::fromcsv("AACCGGTA")) << endl;
    cout << "> test1: " << count_mutations("AACCGGTT", "AAACGGTA", stringlist::fromcsv("AACCGGTA,AACCGCTA,AAACGGTA")) << endl;
    cout << "> test2: " << count_mutations("AAAAACCC", "AACCCCCC", stringlist::fromcsv("AAAACCCC,AAACCCCC,AACCCCCC")) << endl;

    return 0;
}