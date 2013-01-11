#ifndef _STATUS_H_
#define _STATUS_H_

#include <inttypes.h>

/* Time constants - in microseconds */
#define STATUS_SHORT 300
#define STATUS_LONG  900
#define STATUS_BURSTDELAY 50000

void statusTxSymbol(uint8_t symbol);
void statusTxPacket(uint64_t *d, uint8_t rep);

#endif
