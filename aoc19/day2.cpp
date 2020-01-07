#include "harness.h"
#include "intproc.h"


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
    ip.poke(1, noun);
    ip.poke(2, verb);

    ip.run();

    return ip.peek(0);
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

