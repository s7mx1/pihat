/******************************************************************************/
/*              PiHAT routines for Status remote power switches                 */
/*                                                                            */
/*                          S7mx1, 2012                                       */
/******************************************************************************/

#include "status.h"

#include <inttypes.h>
#include <sys/time.h>
#include <unistd.h>
#include "radio.h"

/* Status has 2 different symbols, 0, 1 */
void statusTxSymbol(uint8_t symbol){
	if (symbol == 0) {
            askHigh();
            usleep(STATUS_SHORT);
            askLow();
            usleep(STATUS_LONG);
        }
	else {
	    askHigh();
	    usleep(STATUS_LONG);
            askLow();
            usleep(STATUS_SHORT);
       }
}

void statusTxPacket(uint64_t *d, uint8_t rep){
	uint64_t mask;
	uint8_t symbol_nr;
	uint8_t symbol_nr_max;
	uint8_t p_nr;

	symbol_nr_max = 25;

	for (p_nr = 0; p_nr < rep; p_nr++){

		mask = 0x1000000;

		/* Transmit the packet content */
		for (symbol_nr = 0; symbol_nr<symbol_nr_max; symbol_nr++){
			if ((*d) & mask) statusTxSymbol(1);
			else statusTxSymbol(0);
			mask >>= 1;
		}
                usleep(STATUS_BURSTDELAY);
	}
        usleep(4 * STATUS_BURSTDELAY);
}
