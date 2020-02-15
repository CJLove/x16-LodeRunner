#include "loderunner.h"
#include <conio.h>
#include <cx16.h>

int8_t keyAction()
{
    int8_t act = ACT_UNKNOWN;
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
}