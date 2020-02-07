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

    // Load palette
    result = vload_host("palette.bin",0xf1000);
    if (result) 
        printf("  Loaded palette\n");
    else {
        printf("  Failed to load palette\n");
        return 0;
    }
    
    // Load tiles
    result = vload_host("tiles.bin",0x10000);
    if (result) 
        printf("  Loaded tiles\n");
    else {
        printf("  Failed to load tiles\n");
        return 0;
    }
    
    // Load classic levels to banks 1-5
    result = load_bank_host("classic.bin",1);
    if (result)
        printf("  Loaded classic levels\n");
    else {
        printf("  Failed to load classic levels\n");
        return 0;
    }

    // Load Championship levels to banks 6-7
    result = load_bank_host("champ.bin",6);
    if (result)
        printf("  Loaded championship levels\n");
    else {
        printf("  Failed to load championship levels\n");
        return 0;
    }
    
    // Load Professional levels to banks 8-12
    result = load_bank_host("pro.bin",8);
    if (result)
        printf("  Loaded professional levels\n");
    else {
        printf("  Failed to load professional levels\n");
        return 0;
    }    

    // Load Fanbook levels to banks 13-14
    result = load_bank_host("fanbook.bin",13);
    if (result) 
        printf("  Loaded fanbook levels\n");
    else {
        printf("  Failed to load fanbook levels\n");
        return 0;
    }

    // Load Revenge levels to bank 15
    result = load_bank_host("revenge.bin",15);
    if (result)
        printf("  Loaded revenge levels\n");
    else {
        printf("  Failed to load revenge levels\n");
        return 0;
    }

    return (result != 0);
}