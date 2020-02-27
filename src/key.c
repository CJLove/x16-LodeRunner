#include "loderunner.h"
#include <conio.h>
#include <cx16.h>
#include <joystick.h>
#include <stdio.h>

int8_t keyAction()
{
    int8_t act = ACT_UNKNOWN;
#if 0  
    if (kbhit()) {
        char c = cgetc();
        // TODO: Support customized user key bindings
        switch (c) {
            case 'i':
                act = ACT_UP;
                break;
            case 'm':
                act = ACT_DOWN;
                break;
            case 'j':
                act = ACT_LEFT;
                break;
            case 'k':
                act = ACT_RIGHT;
                break;
            case 'a':
                act = ACT_DIG_LEFT;
                break;
            case 's':
                act = ACT_DIG_RIGHT;
                break;
            default:
                break;
        }
    }
    return act;
#else
    uint8_t joy = joy_read(0);
    if (joy & JOY_BTN_1_MASK) {
        act = ACT_DIG_LEFT;
    } else if (joy & JOY_BTN_2_MASK) {
        act = ACT_DIG_RIGHT;
    } else if (joy & JOY_UP_MASK) {
        act = ACT_UP;
    } else if (joy & JOY_DOWN_MASK) {
        act = ACT_DOWN;
    } else if (joy & JOY_LEFT_MASK) {
        act = ACT_LEFT;
    } else if (joy & JOY_RIGHT_MASK) {
        act = ACT_RIGHT;
    } else if (joy & JOY_BTN_4_MASK) {
        act = ACT_START;
    }
    // else if (joy) {
    //     printf("joy mask 0x%x\n",joy);
    // }
    return act;
#endif
}