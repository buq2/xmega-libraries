#include "axlib/sensors/thermo_max6675.hh"
#include <LUFA/Drivers/Peripheral/SPI.h>

#define SPI_SS_PIN 0b00010000
#define SPI_MOSI_PIN 0b00100000
#define SPI_SCK_PIN 0b10000000

// http://datasheets.maximintegrated.com/en/ds/MAX6675.pdf

using namespace axlib;

ThermoMax6675::ThermoMax6675(const axlib::Port spi_port, const axlib::Port cs_port,
                             const uint8_t cs_pin)
    :
      spi_port_(axlib::GetPort(spi_port)),
      spi_(axlib::GetSpiPort(spi_port)),
      cs_port_(axlib::GetPort(cs_port)),
      cs_pin_(cs_pin),
      temperature_bias_(0)
{
    Setup();
}

bool ThermoMax6675::Setup()
{
    // Setup SPI pins
    spi_port_->DIRSET  = SPI_MOSI_PIN | SPI_SCK_PIN | SPI_SS_PIN;
    spi_port_->PIN4CTRL = PORT_OPC_WIREDANDPULL_gc;

    SetChipSelected(false);

    // Set the actual CS pin (which is not in the SPI port)
    cs_port_->DIRSET = cs_pin_;
    cs_port_->OUTCLR = cs_pin_;

    // Setup SPI
    // It seems that even SPI_SPEED_FCPU_DIV_4  could be usable
    SPI_Init(spi_,
             SPI_SPEED_FCPU_DIV_128  | SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_RISING |
             SPI_SAMPLE_LEADING | SPI_MODE_MASTER);

    return 0;
}

void ThermoMax6675::SetChipSelected(const bool selected)
{
    if (selected) {
        cs_port_->OUTCLR = cs_pin_;
    } else {
        cs_port_->OUTSET = cs_pin_;
    }
}

bool ThermoMax6675::GetTemperatureCelcius(uint16_t *temperature)
{
    SetChipSelected(true);
    uint16_t b1 = SPI_ReceiveByte(spi_)*255;
    b1 += ((uint16_t)SPI_ReceiveByte(spi_));
    b1 = b1>>3;
    b1 /= 4;
    SetChipSelected(false);

    *temperature = b1 - temperature_bias_;

    return true;
}

void ThermoMax6675::SetTemperatureBiasCelcius(const uint16_t bias)
{
    temperature_bias_ = bias;
}
