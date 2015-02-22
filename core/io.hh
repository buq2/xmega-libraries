#ifndef AXLIB_CORE_PORT_HH
#define AXLIB_CORE_PORT_HH

#include <avr/io.h>

namespace axlib {

typedef enum Port_t
{
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_R
} Port;

PORT_t *GetPort(const Port port);
SPI_t *GetSpiPort(const Port port);
TC0_t *GetTimerCounter0(const Port port);
TC1_t *GetTimerCounter1(const Port port);
TC2_t *GetTimerCounter2(const Port port);

} //namespace axlib

#endif //ifndef AXLIB_CORE_PORT_HH
