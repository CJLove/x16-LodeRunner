#include <stdio.h>
#include <cbm.h>
#include <cx16.h>
#include "vload.h"
#include "load.h"

int loadFiles()
{
    int result = 1;
    unsigned char *dest = NULL;

    // TODO: Look at EMULATOR->detect[0] and [1] to detect if running in emulator

    // Initialize 0x1f9c0-0x1ffff to known values
    // uint32_t vera = 0x1f9c0;
    // VERA.address_hi = 0x10;

    // vpoke(0,vera); vera++;
    // for (; vera != 0x20000; vera++) {
    //     VERA.data0 = 0;
    // }

    // // Load blank tilemap
    // result = vload_host("tilemap.bin",0x00000);
    // if (result)
    //     printf("  loaded blank tilemap\n");
    // else {
    //     printf("  failed to load tilemap\n");
    //     return 0;
    // }

    // Load palette
    result = vload_host("palette.bin",0x1fa00);
    if (result) 
        printf("  loaded palette\n");
    else {
        printf("  failed to load palette\n");
        return 0;
    }
    
    // Load tiles
    result = vload_host("tiles.bin",0x10000);
    if (result) 
        printf("  loaded tiles\n");
    else {
        printf("  failed to load tiles\n");
        return 0;
    }

    // Load sprites
    result = vload_host("sprites.bin",0x1e000);
    if (result)
        printf("  loaded sprites\n");
    else {
        printf("  failed to load sprites\n");
        return 0;
    }

    // Load splash tilemap to bank 21
    result = load_bank_host("splash.bin",21);
    if (result) 
        printf("  loaded splash tilemap\n");
    else {
        printf("  failed to load splash tilemap\n");
        return 0;
    }
    
    // Load classic levels to banks 1-5
    result = load_bank_host("classic.bin",1);
    if (result)
        printf("  loaded classic levels\n");
    else {
        printf("  failed to load classic levels\n");
        return 0;
    }

    // Load Championship levels to banks 6-7
    result = load_bank_host("champ.bin",6);
    if (result)
        printf("  loaded championship levels\n");
    else {
        printf("  failed to load championship levels\n");
        return 0;
    }
    
    // Load Professional levels to banks 8-12
    result = load_bank_host("pro.bin",8);
    if (result)
        printf("  loaded professional levels\n");
    else {
        printf("  failed to load professional levels\n");
        return 0;
    }    

    // Load Fanbook levels to banks 13-14
    result = load_bank_host("fanbook.bin",13);
    if (result) 
        printf("  loaded fanbook levels\n");
    else {
        printf("  failed to load fanbook levels\n");
        return 0;
    }

    // Load Revenge levels to bank 15
    result = load_bank_host("revenge.bin",15);
    if (result)
        printf("  loaded revenge levels\n");
    else {
        printf("  failed to load revenge levels\n");
        return 0;
    }

    // Load Custom levels to bank 16
    result = load_bank_host("custom.bin",16);

    return (result != 0);
}
