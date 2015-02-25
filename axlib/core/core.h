#ifndef AXLIB_CORE_CORE_H
#define AXLIB_CORE_CORE_H

#ifdef USE_MCU
#include <avr/pgmspace.h>
#else
#define PROGMEM
#define pgm_read_byte(x) (*(x))
#endif

#ifndef MIN
#define MIN(x,y)((x)<(y)?(x):(y))
#endif

#ifndef MAX
#define MAX(x,y)((x)>(y)?(x):(y))
#endif

#ifndef ABS
#define ABS(x)((x)<0?(-(x)):(x))
#endif

#ifndef NULL
#define NULL 0
#endif

#define BIT_SET(value,bit)(value | (1 << bit))
#define BIT_CLEAR(value,bit)(value & (~(1 << bit)))
#define BIT_TOGGLE(value,bit)(value ^ (1 << bit))

#endif //ifndef AXLIB_CORE_CORE_H
