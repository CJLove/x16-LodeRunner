#include "loderunner.h"
#include <conio.h>
#include <cx16.h>
#include <joystick.h>
#include <stdio.h>

int8_t keyAction()
{
    int8_t act = ACT_UNKNOWN;

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
}