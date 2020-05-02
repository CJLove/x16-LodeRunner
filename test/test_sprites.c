#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which displays all tiles

int main()
{
    int result = 0;
    uint8_t tile = 0;
    uint16_t x = 16;
    uint16_t y = 16;
    uint32_t sprite = 0x11fc00; // Address includes memory increment 1
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    screenConfig();

    // Runner sprite images

    // vpoke() seems to be messing with VERA.data0 behavior
    vpoke((RUNNER_1 >> 5) & 0xff, sprite);      // Attr0
    VERA.data0 = (RUNNER_1 >>13) & 0xf;         // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = y >> 8;                        // Attr5
    VERA.data0 = (3 << 2);                      // Attr6 - z pos in front of layer 1
    VERA.data0 = 0;                             // Attr7

    x += 16;
    sprite += 8;

    vpoke((RUNNER_2 >> 5) & 0xff, sprite);      // Attr0
    VERA.data0 = (RUNNER_2 >> 13) & 0xf;        // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7

    x += 16;
    sprite += 8;

    vpoke((RUNNER_3 >> 5) & 0xff, sprite);      // Attr0
    VERA.data0 = (RUNNER_3 >> 13) & 0xf;        // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7    

    x += 16;
    sprite += 8;

    vpoke((RUNNER_CLIMB_1 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (RUNNER_CLIMB_1 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7    

    x += 16;
    sprite += 8;

    vpoke((RUNNER_CLIMB_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (RUNNER_CLIMB_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7    

    x += 16;
    sprite += 8;

    vpoke((RUNNER_FALLING >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (RUNNER_FALLING >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7    

    x += 16;
    sprite += 8;

    vpoke((RUNNER_RAPPEL_1 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (RUNNER_RAPPEL_1 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                       // Attr2
    VERA.data0 = x >> 8;                         // Attr3
    VERA.data0 = y & 0xff;                       // Attr4
    VERA.data0 = 7>> 8;                          // Attr5
    VERA.data0 = (3<<2);                         // Attr6
    VERA.data0 = 0;                              // Attr7    

    x += 16;
    sprite += 8;

    vpoke((RUNNER_RAPPEL_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (RUNNER_RAPPEL_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                       // Attr2
    VERA.data0 = x >> 8;                         // Attr3
    VERA.data0 = y & 0xff;                       // Attr4
    VERA.data0 = 7>>8;                           // Attr5
    VERA.data0 = (3<<2);                         // Attr6
    VERA.data0 = 0;                              // Attr7

    x += 16;
    sprite += 8;

    vpoke((RUNNER_RAPPEL_3 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (RUNNER_RAPPEL_3 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                       // Attr2
    VERA.data0 = x >> 8;                         // Attr3
    VERA.data0 = y & 0xff;                       // Attr4
    VERA.data0 = 7>> 8;                          // Attr5
    VERA.data0 = (3<<2);                         // Attr6
    VERA.data0 = 0;                              // Attr7    

    // Guard sprite images
    x=16;
    y = 32;
    sprite += 8;

    vpoke((GUARD_1 >> 5) & 0xff, sprite); // Attr0
    VERA.data0 = (GUARD_1 >> 13) & 0xf;   // Attr1
    VERA.data0 = x & 0xff;                // Attr2
    VERA.data0 = x >> 8;                  // Attr3
    VERA.data0 = y & 0xff;                // Attr4
    VERA.data0 = 7>> 8;                   // Attr5
    VERA.data0 = (3<<2);                  // Attr6
    VERA.data0 = 0;                       // Attr7    

    x += 16;
    sprite += 8;

    vpoke((GUARD_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;               // Attr2
    VERA.data0 = x >> 8;                 // Attr3
    VERA.data0 = y & 0xff;               // Attr4
    VERA.data0 = 7>> 8;                  // Attr5
    VERA.data0 = (3<<2);                 // Attr6
    VERA.data0 = 0;                      // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_3 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_3 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;               // Attr2
    VERA.data0 = x >> 8;                 // Attr3
    VERA.data0 = y & 0xff;               // Attr4
    VERA.data0 = 7>> 8;                  // Attr5
    VERA.data0 = (3<<2);                 // Attr6
    VERA.data0 = 0;                      // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_CLIMB_1 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_CLIMB_1 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                     // Attr2
    VERA.data0 = x >> 8;                       // Attr3
    VERA.data0 = y & 0xff;                     // Attr4
    VERA.data0 = 7>> 8;                        // Attr5
    VERA.data0 = (3<<2);                       // Attr6
    VERA.data0 = 0;                            // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_CLIMB_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_CLIMB_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                     // Attr2
    VERA.data0 = x >> 8;                       // Attr3
    VERA.data0 = y & 0xff;                     // Attr4
    VERA.data0 = 7>> 8;                        // Attr5
    VERA.data0 = (3<<2);                       // Attr6
    VERA.data0 = 0;                            // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_FALLING >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_FALLING >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                     // Attr2
    VERA.data0 = x >> 8;                       // Attr3
    VERA.data0 = y & 0xff;                     // Attr4
    VERA.data0 = 7>> 8;                        // Attr5
    VERA.data0 = (3<<2);                       // Attr6
    VERA.data0 = 0;                            // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_RAPPEL_1 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_RAPPEL_1 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_RAPPEL_2 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_RAPPEL_2 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7       

    x += 16;
    sprite += 8;

    vpoke((GUARD_RAPPEL_3 >> 5) & 0xff, sprite);// Attr0
    VERA.data0 = (GUARD_RAPPEL_3 >> 13) & 0xf;  // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = 7>> 8;                         // Attr5
    VERA.data0 = (3<<2);                        // Attr6
    VERA.data0 = 0;                             // Attr7       

    // Enable
    VERA.dc_video |= 0x40;

    while (1) {
        waitvsync();
    }

    return result;
}