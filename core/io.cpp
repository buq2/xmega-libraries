#include "core/io.hh"

namespace axlib {

PORT_t *GetPort(const Port port)
{
    switch(port)  {
    case PORT_A:
        return &PORTA;
    case PORT_B:
        return &PORTB;
    case PORT_C:
        return &PORTC;
    case PORT_D:
        return &PORTD;
    case PORT_E:
        return &PORTE;
    case PORT_R:
        return &PORTR;
    }
    return 0;
}

SPI_t *GetSpiPort(const Port port)
{
    switch(port)  {
    case PORT_C:
        return &SPIC;
    case PORT_D:
        return &SPID;
    default:
        return 0;
    }
    return 0;
}

TC0_t *GetTimerCounter0(const Port port)
{
    switch(port)  {
    case PORT_C:
        return &TCC0;
    case PORT_D:
        return &TCD0;
    case PORT_E:
        return &TCE0;
    default:
        return 0;
    }
    return 0;
}

TC1_t *GetTimerCounter1(const Port port)
{
    switch(port)  {
    case PORT_C:
        return &TCC1;
    case PORT_D:
        return &TCD1;
    default:
        return 0;
    }
    return 0;
}

TC2_t *GetTimerCounter2(const Port port)
{
    switch(port)  {
    case PORT_C:
        return &TCC2;
    case PORT_D:
        return &TCD2;
    default:
        return 0;
    }
    return 0;
}

} //namespace axlib
