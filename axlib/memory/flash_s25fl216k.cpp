#include "axlib/memory/flash_s25fl216k.hh"
#include <LUFA/Drivers/Peripheral/SPI.h>

using namespace axlib;

#define SPI_MOSI_PIN 0b00100000
#define SPI_SCK_PIN 0b10000000

FlashS25Fl216K::FlashS25Fl216K(const Port spi_port, const Port cs_port, const Pin cs_pin)
    :
      spi_(GetSpiPort(spi_port)),
      spi_port_(GetPort(spi_port)),
      cs_port_(GetPort(cs_port)),
      cs_pin_((uint8_t)cs_pin)
{
    // Setup SPI pins
    spi_port_->DIRSET  = SPI_MOSI_PIN | SPI_SCK_PIN;

    // Set the actual CS pin (which is not in the SPI port)
    cs_port_->DIRSET = cs_pin_;
    cs_port_->OUTCLR = cs_pin_;

    SPI_Init(spi_,
             SPI_SPEED_FCPU_DIV_8  | SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_RISING |
             SPI_SAMPLE_TRAILING | SPI_MODE_MASTER);

    SetChipSelected(0);
}

void FlashS25Fl216K::SetChipSelected(const bool selected)
{
    if (selected) {
        cs_port_->OUTCLR = cs_pin_;
    } else {
        cs_port_->OUTSET = cs_pin_;
    }
}

