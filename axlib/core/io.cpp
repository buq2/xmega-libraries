#include "axlib/core/io.hh"

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
    default:
        return 0;
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

TWI_t *GetI2CPort(const Port port)
{
    switch(port)  {
    case PORT_C:
        return &TWIC;
    case PORT_E:
        return &TWIE;
    default:
        return 0;
    }
    return 0;
}

register8_t *GetPortPinControlRegister(const Port port, const Pin pin)
{
    PORT_t *p = GetPort(port);
    if (0 == p) {
        return 0;
    }
    switch(pin) {
    case PIN_0:
        return &p->PIN0CTRL;
    case PIN_1:
        return &p->PIN1CTRL;
    case PIN_2:
        return &p->PIN2CTRL;
    case PIN_3:
        return &p->PIN3CTRL;
    case PIN_4:
        return &p->PIN4CTRL;
    case PIN_5:
        return &p->PIN5CTRL;
    case PIN_6:
        return &p->PIN6CTRL;
    case PIN_7:
        return &p->PIN7CTRL;
    default:
        return 0;
    }
}

} //namespace axlib
