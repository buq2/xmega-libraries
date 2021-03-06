#include "axlib/sensors/altimeter_mpl3115a2.hh"
#include <LUFA/Drivers/Peripheral/TWI.h>
#include "axlib/core/core.hh"

using namespace axlib;

#define I2C_SPEED 400000
#define I2C_TIMEOUT 20
#define MPL3115A2_ADDRESS_READ 0xC1 // 1100 0001
#define MPL3115A2_ADDRESS_WRITE 0xC0 // 1100 0000

#define RETURN_ERROR_IF_ERROR(x) {if (x) {return x;}}
#define RETURN_NULL_IF_ERROR(x) {if (x) {return NULL;}}

AltimeterMPl3114A2::AltimeterMPl3114A2(const Port i2c_port)
    :
      request_data_called_(false),
      configuration_data_(0b10000000),
      altitude_ground_(0.0f),
      i2c_port_(GetI2CPort(i2c_port))
{
}

void AltimeterMPl3114A2::SetOversampleRate(const AltimeterMPl3114A2::OversampleRate rate)
{
    if (rate == OversampleRate1) {
        configuration_data_ = BIT_CLEAR(configuration_data_,3);
        configuration_data_ = BIT_CLEAR(configuration_data_,4);
        configuration_data_ = BIT_CLEAR(configuration_data_,5);
    } else if (rate == OversampleRate2) {
        configuration_data_ = BIT_SET(configuration_data_,3);
        configuration_data_ = BIT_CLEAR(configuration_data_,4);
        configuration_data_ = BIT_CLEAR(configuration_data_,5);
    } else if (rate == OversampleRate4) {
        configuration_data_ = BIT_CLEAR(configuration_data_,3);
        configuration_data_ = BIT_SET(configuration_data_,4);
        configuration_data_ = BIT_CLEAR(configuration_data_,5);
    } else if (rate == OversampleRate8) {
        configuration_data_ = BIT_SET(configuration_data_,3);
        configuration_data_ = BIT_SET(configuration_data_,4);
        configuration_data_ = BIT_CLEAR(configuration_data_,5);
    } else if (rate == OversampleRate16) {
        configuration_data_ = BIT_CLEAR(configuration_data_,3);
        configuration_data_ = BIT_CLEAR(configuration_data_,4);
        configuration_data_ = BIT_SET(configuration_data_,5);
    } else if (rate == OversampleRate32) {
        configuration_data_ = BIT_SET(configuration_data_,3);
        configuration_data_ = BIT_CLEAR(configuration_data_,4);
        configuration_data_ = BIT_SET(configuration_data_,5);
    } else if (rate == OversampleRate64) {
        configuration_data_ = BIT_CLEAR(configuration_data_,3);
        configuration_data_ = BIT_SET(configuration_data_,4);
        configuration_data_ = BIT_SET(configuration_data_,5);
    } else if (rate == OversampleRate128) {
        configuration_data_ = BIT_SET(configuration_data_,3);
        configuration_data_ = BIT_SET(configuration_data_,4);
        configuration_data_ = BIT_SET(configuration_data_,5);
    }
}

uint8_t AltimeterMPl3114A2::Setup()
{
    // Initialize the TWI driver
    TWI_Init(i2c_port_, TWI_BAUD_FROM_FREQ(I2C_SPEED));

    uint8_t address = 0x26;
    uint8_t packet = configuration_data_;
    uint8_t err = TWI_WritePacket(i2c_port_, MPL3115A2_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  &packet, 1);
    RETURN_ERROR_IF_ERROR(err);

    // Enable "data ready" flags
    address = 0x13;
    packet = 0x07;
    err = TWI_WritePacket(i2c_port_, MPL3115A2_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                          &packet, 1);
    RETURN_ERROR_IF_ERROR(err);

    return 0;
}

void AltimeterMPl3114A2::SetMode(const bool altimeter)
{
    if (altimeter) {
        configuration_data_ = BIT_SET(configuration_data_,7);
    } else {
        configuration_data_ = BIT_CLEAR(configuration_data_,7);
    }
}

uint8_t AltimeterMPl3114A2::RequestDataUpdate()
{
    if (request_data_called_) {
        // Already active
        return 0;
    }

    //Set Active
    uint8_t packet = configuration_data_;//0xB9;//configuration_data_;
    packet = BIT_SET(packet, 1); // Immediate update
    packet = BIT_CLEAR(packet, 0); // Auto clear immediat update bit
    uint8_t address = 0x26;
    uint8_t err = TWI_WritePacket(i2c_port_, MPL3115A2_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  &packet, 1);
    RETURN_ERROR_IF_ERROR(err);
    request_data_called_ = true;

    return 0;
}

uint8_t AltimeterMPl3114A2::DataReady()
{
    // Check status register
    uint8_t address = 0;
    uint8_t packet = 0;
    uint8_t err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                    &packet, 1);
    RETURN_ERROR_IF_ERROR(err);

    // 0x08 is 1 if OUT_P or OUT_T has been overwritten
    // this flag is cleared by reading OUT_P_MSB or OUT_T_MSB
    if (packet & 0x08) {
        // Altitude ready
        return 1;
    }
    return 0;
}

uint8_t AltimeterMPl3114A2::GetAltitudeMeters(float *altitude)
{
    uint8_t err = GetRawAltitudeMeters(altitude);
    RETURN_ERROR_IF_ERROR(err);

    *altitude -= altitude_ground_;

    return 0;
}

uint8_t AltimeterMPl3114A2::GetTemperature(float *temperature)
{
    uint8_t OUT_T_MSB;
    uint8_t OUT_T_LSB;
    uint8_t address = 0x04;
    uint8_t err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                         &OUT_T_MSB, 1);
    RETURN_ERROR_IF_ERROR(err);
    request_data_called_ = false; //MSB read -> need to make another request

    address = 0x05;
    err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                         &OUT_T_LSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    const int16_t temp_raw = OUT_T_LSB | (((uint16_t)OUT_T_MSB)<<8);

    *temperature = temp_raw/256.0f;
    previous_temperature_ = *temperature;

    return 0;
}

uint8_t AltimeterMPl3114A2::GetPressurePascals(float *pressure)
{
    uint8_t OUT_P_MSB;
    uint8_t OUT_P_CSB;
    uint8_t OUT_P_LSB;
    uint8_t address = 0x01;
    uint8_t err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &OUT_P_MSB, 1);
    RETURN_ERROR_IF_ERROR(err);
    request_data_called_ = false; //MSB read -> need to make another request

    address = 0x02;
    err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_CSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x03;
    err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_LSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    const int32_t alt_raw = OUT_P_LSB | (((uint32_t)OUT_P_CSB)<<8) | (((uint32_t)OUT_P_MSB)<<16);

    // Convert to altitude in meters
    *pressure = alt_raw/64.0f;
    previous_pressure_ = *pressure;

    return 0;
}

char *AltimeterMPl3114A2::GetAltitudeMetersString()
{
    float alt;
    GetAltitudeMeters(&alt);

    sprintf(altitude_string_,"%f\n\r",alt);
    return altitude_string_;
}

void AltimeterMPl3114A2::ZeroAltitudeLoop()
{
    // It looks like first few samples can have
    // large error(?)
    for(uint8_t i = 0; i < 5; ++i) {
        while (0 != RequestDataUpdate()) {
        }
        uint8_t c = 0;
        while(c < 100 && !DataReady()) {
            _delay_ms(10);
            ++c;
        }
        while (0 != ZeroAltitude()) {
        }
    }
}

uint8_t AltimeterMPl3114A2::ZeroAltitude()
{
    return GetRawAltitudeMeters(&altitude_ground_);
}

uint8_t AltimeterMPl3114A2::GetRawAltitudeMeters(float *altitude)
{
    uint8_t OUT_P_MSB;
    uint8_t OUT_P_CSB;
    uint8_t OUT_P_LSB;
    uint8_t address = 0x01;
    uint8_t err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &OUT_P_MSB, 1);
    RETURN_ERROR_IF_ERROR(err);
    request_data_called_ = false; //MSB read -> need to make another request

    address = 0x02;
    err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_CSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x03;
    err = TWI_ReadPacket(i2c_port_, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_LSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    const int32_t alt_raw = ((uint32_t)OUT_P_LSB<<8) | (((uint32_t)OUT_P_CSB)<<16) | (((uint32_t)OUT_P_MSB)<<24);

    // Convert to altitude in meters
    *altitude = ((float)alt_raw)/65536.0f;
    previous_raw_altitude_ = *altitude;

    return 0;
}
