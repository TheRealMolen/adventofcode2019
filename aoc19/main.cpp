#include "harness.h"

#include <ctime>



// -------------------------------------------------------------------

// -------------------------------------------------------------------

int main()
{
    initcolours();
    srand((unsigned int)time(0));

    cout << GARLAND(2) << "  advent of code 2019  " << GARLAND(2) << endl;



    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

