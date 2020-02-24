#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include <joystick.h>
#include "loderunner.h"
#include "key.h"

// Test application which tests key input

int main()
{
    int result = 0;
    uint8_t tile = 0;
    printf("Loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("Loaded resources successfully\n");
    } else {
        printf("Failed to load all resources\n");
        return result;
    }

    joy_install(cx16_std_joy);

    do {
        int8_t act = keyAction();

        switch (act) {
        case ACT_UNKNOWN: 
            // Don't print anything
            //printf("UNKNOWN\n");
            break;
        case ACT_STOP:
            printf("STOP\n");
            break;
        case ACT_LEFT:
            printf("LEFT\n");
            break;
        case ACT_RIGHT:
            printf("RIGHT\n");
            break;
        case ACT_UP:
            printf("UP\n");
            break;
        case ACT_DOWN:
            printf("DOWN\n");
            break;
        case ACT_DIG_LEFT:
            printf("DIG LEFT\n");
            break;
        case ACT_DIG_RIGHT:
            printf("DIG RIGHT\n");
            break;
        default:
            printf("OTHER: %d\n",act);
            break;
        }

    } while (1);

    return result;
}