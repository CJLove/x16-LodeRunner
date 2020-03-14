#include "x16test.h"
#include "tests.h"
#include "loderunner.h"

#include <stdio.h>

int main()
{
    //loadFiles();

    registerTestModule(testRunnerLeft,"runnerMovingLeft");

    return x16testmain(TEST_NO_EXIT);

}