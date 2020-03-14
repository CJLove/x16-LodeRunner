#include "levels.h"
#include "runner.h"
#include "guard.h"

// Global game state information

// Runner state information
struct runner_t runner;

// Input handler - default to keyAction() which gets input from joystick routine
keyFcn inputHandler = 0;

// Guard state information
struct guard_t guard[MAX_GUARDS];

// Guards currently in holes
struct shakeGuard_t shake[MAX_GUARDS];
