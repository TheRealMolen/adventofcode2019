#include "intproc.h"

#include <iomanip>
#include <map>
#include <sstream>


const int IntProc::operandmodes[] = { 100, 1000, 10000, 100000 };




ostream& operator<<(ostream& os, const IntProc& ip)
{
    for (auto it = ip.mem.begin(); it != ip.mem.begin() + ip.codesize; ++it)
    {
        if (it != ip.mem.begin())
        {
            os << ',';
        }
        os << *it;
    }

    return os;
}


void IntProc::parse(const string& program, vector<word_t>& mem)
{
    string in(program);
    size_t start = 0;
    size_t sep;

    do {
        sep = in.find(',', start);
        if (sep != string::npos)
        {
            in[sep] = '\0';
        }

        word_t i = strtoll(in.c_str() + start, NULL, 10);
        mem.push_back(i);
        start = sep + 1;
    } while (sep != string::npos);
}


IntProc::IntProc(const string& initial) : pc(0), rel(0), inputs({ 1 })
{
    parse(initial, mem);
    codesize = mem.size();
}

IntProc::IntProc(const vector<word_t>& _mem) : pc(0), rel(0), mem(_mem), inputs({ 1 })
{
    codesize = mem.size();
}


void IntProc::set_input(const list<word_t>& inputs)
{
    this->inputs = inputs;
}


bool IntProc::run()
{
    // set up our bss section :)
    if (mem.size() <= (size_t)codesize)
    {
        mem.resize(mem.size() + 2000);
    }

    for (;;)
    {
        auto instr = fetch(pc);
        auto opcode = instr % 100;
        switch (opcode)
        {
        case 1: // add
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            auto addrOut = fetch(pc + 3);
            store(addrOut, opA + opB, calc_op_mode(instr, 2));
            pc += 4;
            break;
        }

        case 2: // mul
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            auto addrOut = fetch(pc + 3);
            store(addrOut, opA * opB, calc_op_mode(instr, 2));
            pc += 4;
            break;
        }

        case 3: // in
        {
            if (!has_input())
            {
                return false;
            }

            auto addrOut = fetch(pc + 1);
            store(addrOut, read_input(), calc_op_mode(instr, 0));
            pc += 2;
            break;
        }

        case 4: // out
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            output(opA);
            pc += 2;
            break;
        }

        case 5: // jnz
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            if (opA)
            {
                pc = opB;
            }
            else
            {
                pc += 3;
            }
            break;
        }

        case 6: // jz
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            if (!opA)
            {
                pc = opB;
            }
            else
            {
                pc += 3;
            }
            break;
        }

        case 7: // slt
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            auto addrOut = fetch(pc + 3);
            store(addrOut, (opA < opB) ? 1 : 0, calc_op_mode(instr, 2));
            pc += 4;
            break;
        }

        case 8: // seq
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            auto addrOut = fetch(pc + 3);
            store(addrOut, (opA == opB) ? 1 : 0, calc_op_mode(instr, 2));
            pc += 4;
            break;
        }

        case 9: // arb
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            rel += opA;
            pc += 2;
            break;
        }

        case 99: // hlt
            return true;

        default:
            throw "IntProc: BAD_INSTR";
            //cerr << "IntProc got a wonky instruction " << instr << "@" << pc << "\nfinal state: " << *this << endl;
            //return false;
        }
    }
}



const char* IntProc::get_mode_str(word_t instr, int opnum)
{
    auto mode = calc_op_mode(instr, opnum);
    if (mode == 0)
        return "@";
    if (mode == 1)
        return "#";
    if (mode == 2)
        return "rel + @";

    throw "that ain't no addressing mode";
}


void IntProc::dump(const list<word_t>& entries) const
{
    map<word_t, string> disasm;
    vector<bool> code;
    code.resize(mem.size(), false);
    list<word_t> entry_points(entries);

    ostringstream os;

    while(!entry_points.empty())
    {
        // this intentionally hides the class variable :/
        word_t pc = entry_points.front();
        entry_points.pop_front();

        bool keeprunning = true;
        while (keeprunning && (size_t)pc < mem.size() && !code[pc])
        {
            auto thisaddr = pc;
            os.str("");
            os.clear();
            os << setw(4) << setfill('0') << thisaddr << setw(0) << "  ";

            auto instr = fetch(pc);
            auto opcode = instr % 100;
            bool badinstr = false;
            switch (opcode)
            {
            case 1: // add
                os << "add " << get_mode_str(instr, 0) << fetch(pc + 1) << ", " << get_mode_str(instr, 1) << fetch(pc + 2) << ", " << get_mode_str(instr, 2) << fetch(pc + 3);
                pc += 4;
                break;

            case 2: // mul
                os << "mul " << get_mode_str(instr, 0) << fetch(pc + 1) << ", " << get_mode_str(instr, 1) << fetch(pc + 2) << ", " << get_mode_str(instr, 2) << fetch(pc + 3);
                pc += 4;
                break;

            case 3: // in
                os << "in  " << get_mode_str(instr, 0) << fetch(pc + 1);
                pc += 2;
                break;

            case 4: // out
                os << "out " << get_mode_str(instr, 0) << fetch(pc + 1);
                pc += 2;
                break;

            case 5: // jnz
            {
                os << "jnz " << get_mode_str(instr, 0) << fetch(pc + 1) << ", " << get_mode_str(instr, 1) << fetch(pc + 2);
                auto dest_mode = calc_op_mode(instr, 1);
                if (dest_mode != 2)
                {
                    auto dest = fetch(fetch(pc + 2), dest_mode);
                    entry_points.push_back(dest);
                }
                else
                {
                    os << "   ; NOTE: unable to guess jump destination";
                }
                pc += 3;
                break;
            }

            case 6: // jz
            {
                os << "jz  " << get_mode_str(instr, 0) << fetch(pc + 1) << ", " << get_mode_str(instr, 1) << fetch(pc + 2);
                auto dest_mode = calc_op_mode(instr, 1);
                if (dest_mode != 2)
                {
                    auto dest = fetch(fetch(pc + 2), dest_mode);
                    entry_points.push_back(dest);
                }
                else
                {
                    os << "   ; NOTE: unable to guess jump destination";
                }
                pc += 3;
                break;
            }

            case 7: // slt
                os << "slt " << get_mode_str(instr, 0) << fetch(pc + 1) << ", " << get_mode_str(instr, 1) << fetch(pc + 2) << ", " << get_mode_str(instr, 2) << fetch(pc + 3);
                pc += 4;
                break;

            case 8: // seq
                os << "seq " << get_mode_str(instr, 0) << fetch(pc + 1) << ", " << get_mode_str(instr, 1) << fetch(pc + 2) << ", " << get_mode_str(instr, 2) << fetch(pc + 3);
                pc += 4;
                break;

            case 9: // arb
                os << "arb " << fetch(pc + 1);
                pc += 2;
                break;

            case 99: // hlt
                os << "hlt";
                pc += 1;
                keeprunning = false;
                break;

            default:
                // might just be data? os << "Unknown instruction " << instr << endl;
                badinstr = true;
                keeprunning = false;
                break;
            }

            if (!badinstr)
            {
                disasm.emplace(thisaddr, move(os.str()));

                while (thisaddr < pc) {
                    code[thisaddr] = true;
                    ++thisaddr;
                }
            }
        }
    }

    // add all the datas
    for (size_t a = 0; a < mem.size(); ++a)
    {
        if (!code[a])
        {
            os.str("");
            os.clear();
            os << setw(4) << a << setw(0) << "    .data " << mem[a];
            disasm.emplace(a, move(os.str()));
        }
    }

    // ...dump the whole lot!
    for (auto entry : disasm)
    {
        cout << entry.second << endl;
    }
}