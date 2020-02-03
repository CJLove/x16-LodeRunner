#include <stdio.h>
#include <cbm.h>
#include <cx16.h>
#include "vload.h"


int loadFiles()
{
    int result = 1;
    unsigned char *dest = NULL;

    // Load palette
    result = vload_host("palette.bin",0xf1000);
    if (result) 
        printf("  Loaded palette\n");
    else
        printf("  Failed to load palette\n");
    
    // Load tiles
    result = vload_host("tiles.bin",0x10400);
    if (result) 
        printf("  Loaded tiles\n");
    else
        printf("  Failed to load tiles\n");
    
    // Load classic levels to bank 1
    VIA1.pra = 1;
    dest = (unsigned char*)0xa000;
    result = cbm_load("classic.bin",8,dest);
    if (result)
        printf("  Loaded classic levels\n");
    else
        printf("  Failed to load classic levels\n");

    return (result != 0);
}