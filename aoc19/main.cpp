#include "harness.h"

#include <ctime>



// -------------------------------------------------------------------

// fuel counter upper
inline int fuel_for_mass(int mass) {
    return max((mass / 3) - 2, 0);
}
inline int total_fuel_for_mass(int mass) {
    int total = 0;
    int lastmass = mass;
    int extra_fuel;
    do {
        extra_fuel = fuel_for_mass(lastmass);
        total += extra_fuel;
        lastmass = extra_fuel;
    } while (extra_fuel > 0);
    return total;
}

int day1(const stringlist& module_sizes)
{
    int total = 0;
    for (const auto& module_size : module_sizes)
    {
        int size = stoi(module_size);
        int fuel = fuel_for_mass(size);
        total += fuel;
    }
    return total;
}
int day1_2(const stringlist& module_sizes)
{
    int total = 0;
    for (const auto& module_size : module_sizes)
    {
        int size = stoi(module_size);
        int fuel = total_fuel_for_mass(size);
        total += fuel;
    }
    return total;
}


// -------------------------------------------------------------------

int main()
{
    initcolours();
    srand((unsigned int)time(0));

    cout << GARLAND(2) << "  advent of code 2019  " << GARLAND(2) << endl;


    test(2, day1(READ("12")));
    test(2, day1(READ("14")));
    test(654, day1(READ("1969")));
    test(33583, day1(READ("100756")));
    gogogo(day1(LOAD(1)));

    test(2, day1_2(READ("14")));
    test(966, day1_2(READ("1969")));
    test(50346, day1_2(READ("100756")));
    gogogo(day1_2(LOAD(1)));


    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

