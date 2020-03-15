#include "x16test.h"
#include "tests.h"
#include "loderunner.h"

#include <stdio.h>

int main()
{
    loadFiles();

    registerTestModule(testRunnerLeft,"runnerMovingLeft");
    registerTestModule(testRunnerRight,"runnerMovingRight");
    registerTestModule(testRunnerUp,"runnerMovingUp");
    registerTestModule(testRunnerDown,"runnerMovingDown");
    registerTestModule(testRunnerFall,"runnerMovingFall");

    return x16testmain(TEST_NO_EXIT);

}