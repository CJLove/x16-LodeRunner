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
    else
        printf("  Failed to load palette\n");
    
    // Load tiles
    result = vload_host("tiles.bin",0x10000);
    if (result) 
        printf("  Loaded tiles\n");
    else
        printf("  Failed to load tiles\n");
    
    // Load classic levels to bank 1
    result = load_bank_host("classic.bin",1);
    if (result)
        printf("  Loaded classic levels\n");
    else
        printf("  Failed to load classic levels\n");

    return (result != 0);
}