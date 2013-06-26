// Author: Przemysław Bągard
// Program: Performance Tests

#include <iostream>
#include "tests.h"

/////////////////////////
using namespace EnvelopeCollide::Functions;
using namespace std;

int main()
{
    cout << "Performance Tests!";

    // initialize random number generator
    srand(time(NULL));

    // collision detection tests for random situation
    testMain(&generateRandom);

    // collision detection tests for two triangles (deprecated function - before optimalization)
    testDeprecated(&generateRandom);

    // collision detection tests for objects with small movement vector length
    testMain(&generateRandomVerySmallVec);

    // pre-filtering tests for every test situation
    testFilters(&generateRandom);
    testFilters(&generateRandomMove);
    testFilters(&generateRandomSmallVec);
    testFilters(&generateRandomVerySmallVec);

    return 0;
}

