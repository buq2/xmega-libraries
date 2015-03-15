#ifndef AXLIB_CORE_PORT_HH
#define AXLIB_CORE_PORT_HH

#include <avr/io.h>

namespace axlib {

typedef enum Port_t
{
    PORT_NOT_USED,
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_R,
} Port;

typedef enum Pin_t
{
    PIN_0 = 1,
    PIN_1 = 1<<1,
    PIN_2 = 1<<2,
    PIN_3 = 1<<3,
    PIN_4 = 1<<4,
    PIN_5 = 1<<5,
    PIN_6 = 1<<6,
    PIN_7 = 1<<7
} Pin;

PORT_t *GetPort(const Port port);
SPI_t *GetSpiPort(const Port port);
TWI_t *GetI2CPort(const Port port);
TC0_t *GetTimerCounter0(const Port port);
TC1_t *GetTimerCounter1(const Port port);
TC2_t *GetTimerCounter2(const Port port);
register8_t *GetPortPinControlRegister(const Port port, const Pin pin);

/// This method will wait for the byte to be sent until
/// the byte is sent, or we are no longer in master mode
/// This function was created because SS pin was used as a
/// button which pulled the SS pin to low when pressed.
/// This causes SPI to become slave if SS pin is in input mode
/// -> byte would never be sent and we would get stuck in
/// normal mode.
/// Will be called if reset_master_mode_automatically_ is true
void SPI_SendByte_MasterCheck(SPI_t* const spi, const uint8_t val);
uint8_t SPI_ReceiveByte_MasterCheck(SPI_t* const spi);
void SPI_CheckMasterMode(SPI_t *spi);

} //namespace axlib

#endif //ifndef AXLIB_CORE_PORT_HH
