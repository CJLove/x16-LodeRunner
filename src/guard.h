#include <stdint.h>
#include "loderunner.h"

struct guard_t {
    // Flag indicating the guard is active
    uint8_t active;

    // Guard x/y coordinates in tiles
    uint8_t x;
    uint8_t y;

    // Guard x/y offests in pixels (-4 .. 4)
    int8_t xOffset;
    int8_t yOffset;

    // x/y coordinates of guard's hole
    uint8_t holePosX;
    uint8_t holePosY;

    // Action
    int8_t action;

    // Index for sprite image
    uint8_t idx;

    // Last direction (left/right)
    uint8_t direction;

    // Animation sequence (shape)
    uint8_t sequence;

    // Guard has gold
    uint8_t hasGold;
};

struct shakeGuard_t {
    // Flag indicating this shake entry is active
    uint8_t active;
    // Guard ID
    uint8_t id;
    // animation index
    uint8_t idx;
    uint8_t pad;
    // count of how many time processGuardShake()
    // has been called for this guard
    uint16_t count;
};

extern struct guard_t guard[MAX_GUARDS];

extern struct shakeGuard_t shake[MAX_GUARDS];

extern uint8_t guardCount;

void clearGuards(void);
void initGuard(uint8_t x, uint8_t y);
int8_t guardId(uint8_t x, uint8_t y);
void guardReborn(uint8_t x, uint8_t y);
void removeFromShake(uint8_t id);

// Entry point from main loop to shake guards prior to 
// them climbing out of holes
void processGuardShake(void);

// Entry point from main loop to move guards
extern void moveGuard(void);

