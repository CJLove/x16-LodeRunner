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
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    joy_install(cx16_std_joy);

    do {
        int8_t act = keyAction();

        switch (act) {
        case ACT_UNKNOWN: 
            break;
        case ACT_STOP:
            printf("stop\n");
            break;
        case ACT_LEFT:
            printf("left\n");
            break;
        case ACT_RIGHT:
            printf("right\n");
            break;
        case ACT_UP:
            printf("up\n");
            break;
        case ACT_DOWN:
            printf("down\n");
            break;
        case ACT_DIG_LEFT:
            printf("dig left\n");
            break;
        case ACT_DIG_RIGHT:
            printf("dig right\n");
            break;
        default:
            printf("other: %d\n",act);
            break;
        }

    } while (1);

    return result;
}