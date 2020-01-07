#include "harness.h"


const int D4_DIGITS = 6;
bool d4_is_code_valid(int code)
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
bool d4_is_code_valider(int code)
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
                } while (i < D4_DIGITS - 2 && digits[i + 1] == run);
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
