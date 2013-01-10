/******************************************************************************/
/*                    Fractional-N synthesizer setup                          */
/*                                                                            */
/* This code is nearly a direct copy of the PiFm-project                      */
/* by Oliver Mattos and Oskar Weigl.                                          */
/*                                                                            */
/*                          Jon Petter Skagmo, 2012                           */
/******************************************************************************/

#include "radio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

void setup_fm(){
    allof7e = (unsigned *)mmap(
                  NULL,
                  0x01000000,  //len
                  PROT_READ|PROT_WRITE,
                  MAP_SHARED,
                  mem_fd,
                  0x20000000  //base
              );

    if ((int)allof7e==-1) exit(-1);

    SETBIT(GPFSEL0 , 14);
    CLRBIT(GPFSEL0 , 13);
    CLRBIT(GPFSEL0 , 12);

    struct GPCTL setupword = {6/*SRC*/, 1, 0, 0, 0, 1,0x5a};

    ACCESS(CM_GP0CTL) = *((int*)&setupword);
}

//
// Set up a memory regions to access GPIO
//
void setup_io(){
    /* open /dev/mem */
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("can't open /dev/mem \n");
        exit (-1);
    }

    /* mmap GPIO */

    // Allocate MAP block
    if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
        printf("allocation error \n");
        exit (-1);
    }

    // Make sure pointer is on 4K boundary
    if ((unsigned long)gpio_mem % PAGE_SIZE)
        gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

    // Now map it
    gpio_map = (unsigned char *)mmap(
                   gpio_mem,
                   BLOCK_SIZE,
                   PROT_READ|PROT_WRITE,
                   MAP_SHARED|MAP_FIXED,
                   mem_fd,
                   GPIO_BASE
               );

    if ((long)gpio_map < 0) {
        printf("mmap error %d\n", (int)gpio_map);
        exit (-1);
    }

    // Always use volatile pointer!
    gpio = (volatile unsigned *)gpio_map;
} // setup_io

/* Added functions to enable and disable carrier */

void askHigh(){
	struct GPCTL setupword = {6/*SRC*/, 1, 0, 0, 0, 1,0x5a};	// Set CM_GP0CTL.ENABLE to 1
    ACCESS(CM_GP0CTL) = *((int*)&setupword);
}

void askLow(){
	struct GPCTL setupword = {6/*SRC*/, 0, 0, 0, 0, 1,0x5a};	// Set CM_GP0CTL.ENABLE to 0
    ACCESS(CM_GP0CTL) = *((int*)&setupword);
}
