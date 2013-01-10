#ifndef _NEXA_H_
#define _NEXA_H_

#include <inttypes.h>

/* Time constants - in microseconds */
#define NEXA_HIGH 204
#define NEXA_LOWA 272
#define NEXA_LOWB 1326
#define NEXA_LOWC 2720
#define NEXA_BURSTDELAY 10000

void nexaTxSymbol(uint8_t symbol);
void nexaTxPacket(uint64_t *d, uint8_t dim_packet_flag, uint8_t rep);

#endif
