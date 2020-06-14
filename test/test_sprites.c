#include "loderunner.h"
#include <conio.h>
#include <cx16.h>
#include <stdio.h>
#include <unistd.h>
#include <vera.h>

// Test application which displays all tiles

int main()
{
    int result = 0;
    uint8_t tile = 0;
    uint16_t x = 16;
    uint16_t y = 16;
    uint32_t sprite = ((uint32_t)VERA_INC_1 << 16) | SPRITE_ATTR0;

    printf("loading resources...\n");

    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    }
    else {
        printf("failed to load all resources\n");
        return result;
    }

    screenConfig();

    // Runner sprite images
    vpoke(SPRITE_ADDR_L(RUNNER_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);              // Attr2
    VERA.data0 = SPRITE_X_H(x);              // Attr3
    VERA.data0 = SPRITE_Y_L(y);              // Attr4
    VERA.data0 = SPRITE_Y_H(y);              // Attr5
    VERA.data0 = SPRITE_LAYER1;              // Attr6 - z pos in front of layer 1
    VERA.data0 = 0;                          // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);              // Attr2
    VERA.data0 = SPRITE_X_H(x);              // Attr3
    VERA.data0 = SPRITE_Y_L(y);              // Attr4
    VERA.data0 = SPRITE_Y_H(y);              // Attr5
    VERA.data0 = SPRITE_LAYER1;              // Attr6
    VERA.data0 = 0;                          // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_3), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_3);    // Attr1
    VERA.data0 = SPRITE_X_L(x);              // Attr2
    VERA.data0 = SPRITE_X_H(x);              // Attr3
    VERA.data0 = SPRITE_Y_L(y);              // Attr4
    VERA.data0 = SPRITE_Y_H(y);              // Attr5
    VERA.data0 = SPRITE_LAYER1;              // Attr6
    VERA.data0 = 0;                          // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_CLIMB_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_CLIMB_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                    // Attr2
    VERA.data0 = SPRITE_X_H(x);                    // Attr3
    VERA.data0 = SPRITE_Y_L(y);                    // Attr4
    VERA.data0 = SPRITE_Y_H(y);                    // Attr5
    VERA.data0 = SPRITE_LAYER1;                    // Attr6
    VERA.data0 = 0;                                // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_CLIMB_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_CLIMB_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                    // Attr2
    VERA.data0 = SPRITE_X_H(x);                    // Attr3
    VERA.data0 = SPRITE_Y_L(y);                    // Attr4
    VERA.data0 = SPRITE_Y_H(y);                    // Attr5
    VERA.data0 = SPRITE_LAYER1;                    // Attr6
    VERA.data0 = 0;                                // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_FALLING), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_FALLING);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                    // Attr2
    VERA.data0 = SPRITE_X_H(x);                    // Attr3
    VERA.data0 = SPRITE_Y_L(y);                    // Attr4
    VERA.data0 = SPRITE_Y_H(y);                    // Attr5
    VERA.data0 = SPRITE_LAYER1;                    // Attr6
    VERA.data0 = 0;                                // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_RAPPEL_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_RAPPEL_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                     // Attr2
    VERA.data0 = SPRITE_X_H(x);                     // Attr3
    VERA.data0 = SPRITE_Y_L(y);                     // Attr4
    VERA.data0 = SPRITE_Y_H(y);                     // Attr5
    VERA.data0 = SPRITE_LAYER1;                     // Attr6
    VERA.data0 = 0;                                 // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_RAPPEL_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_RAPPEL_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                     // Attr2
    VERA.data0 = SPRITE_X_H(x);                     // Attr3
    VERA.data0 = SPRITE_Y_L(y);                     // Attr4
    VERA.data0 = SPRITE_Y_H(y);                     // Attr5
    VERA.data0 = SPRITE_LAYER1;                     // Attr6
    VERA.data0 = 0;                                 // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(RUNNER_RAPPEL_3), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(RUNNER_RAPPEL_3);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                     // Attr2
    VERA.data0 = SPRITE_X_H(x);                     // Attr3
    VERA.data0 = SPRITE_Y_L(y);                     // Attr4
    VERA.data0 = SPRITE_Y_H(y);                     // Attr5
    VERA.data0 = SPRITE_LAYER1;                     // Attr6
    VERA.data0 = 0;                                 // Attr7

    // Guard sprite images
    x = 16;
    y = 32;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);             // Attr2
    VERA.data0 = SPRITE_X_H(x);             // Attr3
    VERA.data0 = SPRITE_Y_L(y);             // Attr4
    VERA.data0 = SPRITE_Y_H(y);             // Attr5
    VERA.data0 = SPRITE_LAYER1;             // Attr6
    VERA.data0 = 0;                         // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);             // Attr2
    VERA.data0 = SPRITE_X_H(x);             // Attr3
    VERA.data0 = SPRITE_Y_L(y);             // Attr4
    VERA.data0 = SPRITE_Y_H(y);             // Attr5
    VERA.data0 = SPRITE_LAYER1;             // Attr6
    VERA.data0 = 0;                         // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_3), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_3);    // Attr1
    VERA.data0 = SPRITE_X_L(x);             // Attr2
    VERA.data0 = SPRITE_X_H(x);             // Attr3
    VERA.data0 = SPRITE_Y_L(y);             // Attr4
    VERA.data0 = SPRITE_Y_H(y);             // Attr5
    VERA.data0 = SPRITE_LAYER1;             // Attr6
    VERA.data0 = 0;                         // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_CLIMB_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_CLIMB_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                   // Attr2
    VERA.data0 = SPRITE_X_H(x);                   // Attr3
    VERA.data0 = SPRITE_Y_L(y);                   // Attr4
    VERA.data0 = SPRITE_Y_H(y);                   // Attr5
    VERA.data0 = SPRITE_LAYER1;                   // Attr6
    VERA.data0 = 0;                               // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_CLIMB_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_CLIMB_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                   // Attr2
    VERA.data0 = SPRITE_X_H(x);                   // Attr3
    VERA.data0 = SPRITE_Y_L(y);                   // Attr4
    VERA.data0 = SPRITE_Y_H(y);                   // Attr5
    VERA.data0 = SPRITE_LAYER1;                   // Attr6
    VERA.data0 = 0;                               // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_FALLING), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_FALLING);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                   // Attr2
    VERA.data0 = SPRITE_X_H(x);                   // Attr3
    VERA.data0 = SPRITE_Y_L(y);                   // Attr4
    VERA.data0 = SPRITE_Y_H(y);                   // Attr5
    VERA.data0 = SPRITE_LAYER1;                   // Attr6
    VERA.data0 = 0;                               // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_RAPPEL_1), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_RAPPEL_1);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                    // Attr2
    VERA.data0 = SPRITE_X_H(x);                    // Attr3
    VERA.data0 = SPRITE_Y_L(y);                    // Attr4
    VERA.data0 = SPRITE_Y_H(y);                    // Attr5
    VERA.data0 = SPRITE_LAYER1;                    // Attr6
    VERA.data0 = 0;                                // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_RAPPEL_2), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_RAPPEL_2);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                    // Attr2
    VERA.data0 = SPRITE_X_H(x);                    // Attr3
    VERA.data0 = SPRITE_Y_L(y);                    // Attr4
    VERA.data0 = SPRITE_Y_H(y);                    // Attr5
    VERA.data0 = SPRITE_LAYER1;                    // Attr6
    VERA.data0 = 0;                                // Attr7

    x += 16;
    sprite += 8;

    vpoke(SPRITE_ADDR_L(GUARD_RAPPEL_3), sprite);  // Attr0
    VERA.data0 = SPRITE_ADDR_H(GUARD_RAPPEL_3);    // Attr1
    VERA.data0 = SPRITE_X_L(x);                    // Attr2
    VERA.data0 = SPRITE_X_H(x);                    // Attr3
    VERA.data0 = SPRITE_Y_L(y);                    // Attr4
    VERA.data0 = SPRITE_Y_H(y);                    // Attr5
    VERA.data0 = SPRITE_LAYER1;                    // Attr6
    VERA.data0 = 0;                                // Attr7

    // Enable sprites
    vera_sprites_enable(1);

    while (1) {
        waitvsync();
    }

    return result;
}