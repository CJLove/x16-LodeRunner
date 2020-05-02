#include <stdint.h>

struct runner_t {
    // Runner x/y coordinates in tiles
    uint8_t x;  // 0-27
    uint8_t y;  // 0-15

    // Runner x/y offsets in pixels (-4 .. 4)
    int8_t xOffset;
    int8_t yOffset;

    // Action
    int8_t action;

    // Index for sprite image
    uint8_t idx;

    // Last direction (left/right)
    uint8_t direction;

    // Animation sequence (shape)
    uint8_t sequence;
};

typedef int8_t (*keyFcn)(void);

extern keyFcn inputHandler;

extern struct runner_t runner;

extern void clearRunner(void);
extern void initRunner(uint8_t x, uint8_t y);

extern void removeGold(uint8_t x, uint8_t y);
extern void addGold(uint8_t x, uint8_t y);
extern void decGold(void);

extern void setRunnerDead(void);

// Entry point from main loop to move the runner
extern void moveRunner(void);

// Entry point from main loop to fill holes
extern void processFillHole(void);

// Entry point from main loop to dig hole
extern uint8_t isDigging(void);
extern void processDigHole(void);
