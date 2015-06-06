#include "axlib/sensors/digipot_mcp4017t.hh"
#include <LUFA/Drivers/Peripheral/TWI.h>

using namespace axlib;

#define I2C_SPEED 400000
#define I2C_TIMEOUT 20
#define MCP4017T_ADDRESS_READ 0x5F // 01011111
#define MCP4017T_ADDRESS_WRITE 0x5E // 01011110

#define RETURN_ERROR_IF_ERROR(x) {if (x) {return x;}}
#define RETURN_NULL_IF_ERROR(x) {if (x) {return NULL;}}

DigipotMcp4017T::DigipotMcp4017T(const Port i2c_port)
    :
      i2c_port_(GetI2CPort(i2c_port))
{

}

uint8_t DigipotMcp4017T::SetValue(const uint8_t val)
{
    uint8_t zeros = 0;
    uint8_t err = TWI_WritePacket(i2c_port_, MCP4017T_ADDRESS_WRITE, I2C_TIMEOUT, &zeros, 0,
                                  &val, 1);
    RETURN_ERROR_IF_ERROR(err);
}

uint8_t DigipotMcp4017T::Setup()
{
    // Initialize the TWI driver
    TWI_Init(i2c_port_, TWI_BAUD_FROM_FREQ(I2C_SPEED));

    return 0;
}
