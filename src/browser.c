#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which allows browsing of all loaded levels

int main()
{
    int result = 0;
    unsigned char level = 2;
    printf("Loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("Loaded resources successfully\n");
    } else {
        printf("Failed to load all resources\n");
        return result;
    }

    // dumpLevel(1,1,0);
    // dumpLevel(1,1,1);
    // dumpLevel(1,1,2);
    // dumpLevel(1,1,3);

    screenConfig();

    displayLevel(1,level);


    //completeLevel();
    do {
        if (kbhit()) {
            char c = cgetc();
            if (c == 'c') {
                completeLevel();
            }
            else if (c == '+') {
                level++;
                displayLevel(1,level);
            }
            else if (c == '-') {
                if (level > 0)  {
                    level--;
                    displayLevel(1,level);
                }
            }
        }

        sleep(1);
    } while (1);
    cgetc();
    return result;
}