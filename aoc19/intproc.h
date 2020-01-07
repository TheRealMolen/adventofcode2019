#pragma once

#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


class IntProcSymbols;

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

    string display_operand(int opnum, word_t instr_pc, const IntProcSymbols& sym, bool is_data) const;


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

    void dump(const IntProcSymbols& sym) const;
    void dump(ostream& os, const IntProcSymbols& sym) const;

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
    word_t read_final_output()
    {
        auto o = outputs.back();
        outputs.pop_back();
        return o;
    }
    string read_output_string()
    {
        string s;
        s.reserve(outputs.size());
        for (auto c : outputs)
        {
            if (c > 255)
                cerr << "WARNING: losing output " << c << " while converting to string" << endl;
            s.push_back((char)c);
        }
        return s;
    }

};



class IntProcSymbols
{
public:
    typedef IntProc::word_t word_t;

    map<word_t, string> variables;
    map<word_t, string> locations;

    IntProcSymbols(const map<word_t, string>& _variables, const map<word_t, string>& _locations)
        : variables(_variables)
        , locations(_locations)
    {
        if (locations.find(0) == locations.end())
        {
            locations[0] = "main";
        }
    }

    IntProcSymbols(const list<word_t>& entries)
    {
        for (auto location : entries)
        {
            ostringstream os;
            os << "?? @" << location;
            locations[location] = os.str();
        }
        if (locations.find(0) == locations.end())
        {
            locations[0] = "main";
        }
    }
};



void ip_dump(const string& initial, const IntProcSymbols& syms);
void ip_disasm(const string& program, const string& filename, const IntProcSymbols& syms);

