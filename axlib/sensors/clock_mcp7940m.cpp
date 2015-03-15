#include "axlib/sensors/clock_mcp7940m.hh"
#include <LUFA/Drivers/Peripheral/TWI.h>
#include "axlib/core/core.h"

using namespace axlib;

#define I2C_SPEED 400000
#define I2C_TIMEOUT 20
#define MCP7940M_ADDRESS_READ 0xDF // 11011111
#define MCP7940M_ADDRESS_WRITE 0xDE // 11011110

#define RETURN_ERROR_IF_ERROR(x) {if (x) {return x;}}
#define RETURN_NULL_IF_ERROR(x) {if (x) {return NULL;}}

ClockMcp7940M::ClockMcp7940M(const Port i2c_port)
    :
      i2c_port_(GetI2CPort(i2c_port)),
      previous_interrupt_polarity_(0)
{

}

uint8_t ClockMcp7940M::Setup()
{
    // Initialize the TWI driver
    TWI_Init(i2c_port_, TWI_BAUD_FROM_FREQ(I2C_SPEED));

    // Start oscillator if not already running
    uint8_t address = 0x00;
    uint8_t seconds_packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &seconds_packed, 1);
    RETURN_ERROR_IF_ERROR(err);
    // MSB is "oscillator is on" bit
    if (!(seconds_packed & 0b10000000)) {
        seconds_packed |= 0b10000000;
        uint8_t err = TWI_WritePacket(i2c_port_, MCP7940M_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                      &seconds_packed, 1);
        RETURN_ERROR_IF_ERROR(err);
    }

    return 0;
}

uint8_t ClockMcp7940M::ReadSeconds(uint8_t *seconds)
{
    uint8_t address = 0x00;
    uint8_t packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    *seconds = ParseSecond(packed);
    return 0;
}

uint8_t ClockMcp7940M::ReadMinutes(uint8_t *minutes)
{
    uint8_t address = 0x01;
    uint8_t packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    *minutes = ParseMinutes(packed);
    return 0;
}

uint8_t ClockMcp7940M::ReadHours(uint8_t *hours)
{
    uint8_t address = 0x02;
    uint8_t packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    *hours = ParseHours(packed);
    return 0;
}

uint8_t ClockMcp7940M::ReadDay(uint8_t *day)
{
    uint8_t address = 0x04;
    uint8_t packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    *day = ParseDay(packed);
    return 0;
}

uint8_t ClockMcp7940M::ReadMonth(uint8_t *month)
{
    uint8_t address = 0x05;
    uint8_t packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    *month = ParseMonth(packed);
    return 0;
}

uint8_t ClockMcp7940M::ReadYear(uint8_t *year)
{
    uint8_t address = 0x06;
    uint8_t packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    *year = ParseYear(packed);
    return 0;
}

uint8_t ClockMcp7940M::SetDate(const uint8_t year, const uint8_t month, const uint8_t day)
{

}

char *ClockMcp7940M::GetTimeString()
{
    uint8_t seconds, minutes, hours;
    uint8_t day,month,year;
    ReadSeconds(&seconds);
    ReadMinutes(&minutes);
    ReadHours(&hours);
    ReadDay(&day);
    ReadMonth(&month);
    ReadYear(&year);
    sprintf(time_string_,"20%02d-%02d-%02d - %02d:%02d:%02d\n\r",year,month,day,hours,minutes,seconds);
    return time_string_;
}

uint8_t ClockMcp7940M::SetInterruptForNextSecond()
{
    uint8_t sec = 0;
    uint8_t err = ReadSeconds(&sec);
    RETURN_ERROR_IF_ERROR(err);

    err = SetAlarmForSeconds((sec+1)%60, !previous_interrupt_polarity_, 0x0A, 0x0D);
    RETURN_ERROR_IF_ERROR(err);

    // Set second alarm for next second (just in case the seconds already rolled)
    err = SetAlarmForSeconds((sec+2)%60, !previous_interrupt_polarity_, 0x11, 0x0D);
    RETURN_ERROR_IF_ERROR(err);

    previous_interrupt_polarity_ = !previous_interrupt_polarity_;

    return 0;
}

uint8_t ClockMcp7940M::Setup1HzSquareWave()
{
    // Start oscillator if not already running
    uint8_t address = 0x07;
    uint8_t packed = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MCP7940M_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    packed = BIT_SET(packed,6); //Enable square wave
    packed = BIT_CLEAR(packed,2); //Enable square wave
    packed = BIT_CLEAR(packed,1); //1Hz
    packed = BIT_CLEAR(packed,0); //1Hz

    err = TWI_WritePacket(i2c_port_, MCP7940M_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                      &packed, 1);

    RETURN_ERROR_IF_ERROR(err);
}

uint8_t ClockMcp7940M::SetAlarmForSeconds(const uint8_t sec, const bool polarity,
                                          const uint8_t seconds_reg, const uint8_t masks_reg)
{
    uint8_t packed = PackSecond(sec);
    uint8_t err = TWI_WritePacket(i2c_port_, MCP7940M_ADDRESS_WRITE, I2C_TIMEOUT, &seconds_reg, 1,
                                  &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    // TODO: Proper masking of the alarm. Now weekday alaram is overwritten
    // Polarity 0, alarm every second, alarm did not occur, zero weekday
    packed = 0b00000000;
    if (polarity) {
        packed = BIT_SET(packed, 7);
    }

    err = TWI_WritePacket(i2c_port_, MCP7940M_ADDRESS_WRITE, I2C_TIMEOUT, &masks_reg, 1,
                                      &packed, 1);
    RETURN_ERROR_IF_ERROR(err);

    return 0;
}

uint8_t ClockMcp7940M::ParseSecond(const uint8_t packed)
{
    return (packed & 0b00001111) + ((packed >> 4) & 0b00000111)*10;
}

uint8_t ClockMcp7940M::ParseMinutes(const uint8_t packed)
{
    return (packed & 0b00001111) + ((packed >> 4) & 0b00000111)*10;
}

uint8_t ClockMcp7940M::ParseHours(const uint8_t packed)
{
    return (packed & 0b00001111) + ((packed >> 4) & 0b00000001)*10;
}

uint8_t ClockMcp7940M::ParseDay(const uint8_t packed)
{
    return (packed & 0b00001111) + ((packed >> 4) & 0b00000011)*10;
}

uint8_t ClockMcp7940M::ParseMonth(const uint8_t packed)
{
    return (packed & 0b00001111) + ((packed >> 4) & 0b00000001)*10;
}

uint8_t ClockMcp7940M::ParseYear(const uint8_t packed)
{
    return (packed & 0b00001111) + ((packed >> 4) & 0b00001111)*10;
}

uint8_t ClockMcp7940M::PackSecond(const uint8_t data)
{
    return (data/10)<<4 & (data%10);
}


