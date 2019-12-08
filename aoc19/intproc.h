#pragma once

#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


class IntProc
{
public:
    typedef int64_t word_t;

private:
    word_t pc;
    vector<word_t> mem;
    list<word_t> inputs;

    friend ostream& operator<<(ostream& os, const IntProc& ip);

public:
    static void parse(const string& program, vector<word_t>& mem);

    IntProc(const string& initial);
    IntProc(const vector<word_t>& mem);


    void append_input(word_t input)
    {
        inputs.push_back(input);
    }
    void set_input(const list<word_t>& inputs);
    bool run();

    void dump() const;




    word_t fetch(word_t addr, int mode = 0) const
    {
        // mode == 1 means immediate
        if (mode)
        {
            return addr;
        }

        if (addr < 0 || (size_t)addr >= mem.size())
        {
            throw "IntProc:ADDR_VIOLATION";
        }
        return mem[(size_t)addr];
    }

    void store(word_t addr, word_t val)
    {
        if (addr < 0 || (size_t)addr >= mem.size())
        {
            throw "IntProc:ADDR_VIOLATION";
        }
        mem[(size_t)addr] = val;
    }

    static const int operandmodes[];
    static int calc_op_mode(word_t instr, int opnum)
    {
        return (instr / operandmodes[opnum]) % 10;
    }

    bool has_input() const
    {
        return !inputs.empty();
    }
    word_t read_input()
    {
        word_t input = inputs.front();
        inputs.pop_front();
        return input;
    }

    word_t last_output;
    void output(word_t val)
    {
        if (val)
        {
            //cout << "IntProc   OUTPUT: " << val << "   @ pc " << pc << endl;
        }
        last_output = val;
    }

};