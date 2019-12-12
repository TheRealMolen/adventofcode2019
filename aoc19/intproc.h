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
    word_t codesize;
    word_t rel;
    vector<word_t> mem;
    list<word_t> inputs;
    list<word_t> outputs;

    friend ostream& operator<<(ostream& os, const IntProc& ip);
    static const char* get_mode_str(word_t instr, int opnum);


    word_t fetch(word_t addr, int mode = 0) const
    {
        // mode == 1 means immediate
        if (mode == 1)
        {
            return addr;
        }

        // mode == 2 means offset by rel register
        if (mode == 2)
        {
            addr += rel;
        }

        if (addr < 0 || (size_t)addr >= mem.size())
        {
            throw "IntProc:ADDR_VIOLATION";
        }
        return mem[(size_t)addr];
    }

    void store(word_t addr, word_t val, int mode)
    {
        // mode == 2 means offset by rel register
        if (mode == 2)
        {
            addr += rel;
        }

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

    void output(word_t val)
    {
        outputs.push_back(val);
    }

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

    word_t peek(word_t addr) const
    {
        return fetch(addr, 0);
    }
    void poke(word_t addr, word_t val)
    {
        store(addr, val, 0);
    }

    void dump(const list<word_t>& entries = { 0 }) const;

    bool has_output() const
    {
        return !outputs.empty();
    }
    word_t read_output()
    {
        auto o = outputs.front();
        outputs.pop_front();
        return o;
    }

};