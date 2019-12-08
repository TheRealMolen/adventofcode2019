#include "intproc.h"

const int IntProc::operandmodes[] = { 100, 1000, 10000 };




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

        int i = atoi(in.c_str() + start);
        mem.push_back(i);
        start = sep + 1;
    } while (sep != string::npos);
}


IntProc::IntProc(const string& initial) : pc(0), inputs({ 1 })
{
    parse(initial, mem);
}

IntProc::IntProc(const vector<word_t>& mem) : pc(0), mem(mem), inputs({ 1 })
{
    /**/
}


void IntProc::set_input(const list<word_t>& inputs)
{
    this->inputs = inputs;
}


bool IntProc::run()
{
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
            store(addrOut, opA + opB);
            pc += 4;
            break;
        }

        case 2: // mul
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            auto addrOut = fetch(pc + 3);
            store(addrOut, opA * opB);
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
            store(addrOut, read_input());
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

        case 5: // jt
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

        case 6: // jnt
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
            store(addrOut, (opA < opB) ? 1 : 0);
            pc += 4;
            break;
        }

        case 8: // seq
        {
            auto opA = fetch(fetch(pc + 1), calc_op_mode(instr, 0));
            auto opB = fetch(fetch(pc + 2), calc_op_mode(instr, 1));
            auto addrOut = fetch(pc + 3);
            store(addrOut, (opA == opB) ? 1 : 0);
            pc += 4;
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


void IntProc::dump() const
{
    // this intentionally hides the class variable :/
    word_t pc = 0;

    for (;;)
    {
        cout << pc << "\t";

        auto instr = fetch(pc);
        auto opcode = instr % 100;
        switch (opcode)
        {
        case 1: // add
            cout << "add " << (calc_op_mode(instr, 0) ? "#" : "") << fetch(pc + 1) << ", " << (calc_op_mode(instr, 1) ? "#" : "") << fetch(pc + 2) << ", " << fetch(pc + 3) << endl;
            pc += 4;
            break;

        case 2: // mul
            cout << "mul " << (calc_op_mode(instr, 0) ? "#" : "") << fetch(pc + 1) << ", " << (calc_op_mode(instr, 1) ? "#" : "") << fetch(pc + 2) << ", " << fetch(pc + 3) << endl;
            pc += 4;
            break;

        case 3: // in
            cout << "in  " << fetch(pc + 1) << endl;
            pc += 2;
            break;

        case 4: // out
            cout << "out " << fetch(pc + 1) << endl;
            pc += 2;
            break;

        case 5: // jt
            cout << "jt  " << (calc_op_mode(instr, 0) ? "#" : "") << fetch(pc + 1) << ", " << (calc_op_mode(instr, 1) ? "#" : "") << fetch(pc + 2) << endl;
            pc += 3;
            break;

        case 6: // jnt
            cout << "jnt " << (calc_op_mode(instr, 0) ? "#" : "") << fetch(pc + 1) << ", " << (calc_op_mode(instr, 1) ? "#" : "") << fetch(pc + 2) << endl;
            pc += 3;
            break;

        case 7: // slt
            cout << "slt " << (calc_op_mode(instr, 0) ? "#" : "") << fetch(pc + 1) << ", " << (calc_op_mode(instr, 1) ? "#" : "") << fetch(pc + 2) << ", " << fetch(pc + 3) << endl;
            pc += 4;
            break;

        case 8: // seq
            cout << "seq " << (calc_op_mode(instr, 0) ? "#" : "") << fetch(pc + 1) << ", " << (calc_op_mode(instr, 1) ? "#" : "") << fetch(pc + 2) << ", " << fetch(pc + 3) << endl;
            pc += 4;
            break;

        case 99: // hlt
            cout << "hlt" << endl;
            cout << ".data   ";
            for (pc++; pc < (word_t)mem.size(); pc++)
            {
                cout << fetch(pc) << ' ';
            }
            cout << endl;
            break;

        default:
            cout << "Unknown instruction " << instr << endl;
            return;
        }

        if (pc >= (word_t)mem.size())
        {
            cout << "-------------------" << endl;
            break;
        }
    }
}