#include "axlib/displays/display_sharp.hh"
#include <LUFA/Drivers/Peripheral/SPI.h>
#include <util/delay.h>
#include "axlib/core/core.hh"

using namespace axlib;

#define SPI_SS_PIN 0b00010000
#define SPI_MOSI_PIN 0b00100000
#define SPI_SCK_PIN 0b10000000

DisplaySharp::DisplaySharp(const uint8_t width, const uint8_t height,
                           const Port spi_port,
                           const Port cs_port, const uint8_t cs_pin,
                           const Port excomin_port, const uint8_t excomin_pin,
                           const Port display_on_port, const uint8_t display_on_pin)
    :
      width_(width),
      height_(height),
      spi_port_(GetPort(spi_port)),
      spi_(GetSpiPort(spi_port)),
      cs_port_(GetPort(cs_port)),
      cs_pin_(cs_pin),
      excomin_port_(GetPort(excomin_port)),
      excomin_pin_(excomin_pin),
      display_on_port_(GetPort(display_on_port)),
      display_on_pin_(display_on_pin),
      vbit_(false),
      reset_master_mode_automatically_(true)
{

}

bool DisplaySharp::Setup()
{
    // Setup SPI pins
    spi_port_->DIRSET  = SPI_MOSI_PIN | SPI_SCK_PIN;
    // We do not use ss pin
    // SPI_SS_PIN
    // spi_port_->PIN4CTRL = PORT_OPC_WIREDANDPULL_gc;

    SetChipSelected(0);

    // Disable SPI interrupts (we are polling)
    spi_->INTCTRL = 0;

    // Set the actual CS pin (which is not in the SPI port)
    cs_port_->DIRSET = cs_pin_;
    cs_port_->OUTCLR = cs_pin_;

    // And disp on pin
    display_on_port_->DIRSET = display_on_pin_;

    // Setup SPI
    // It seems that even SPI_SPEED_FCPU_DIV_4  could be usable
    SPI_Init(spi_,
             SPI_SPEED_FCPU_DIV_128  | SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_RISING |
             SPI_SAMPLE_TRAILING | SPI_MODE_MASTER);

    // Setup VCOM signal
    if (NULL != excomin_port_) {
        excomin_port_->DIRSET = excomin_pin_;
        excomin_port_->OUTCLR = excomin_pin_;
    }

    return 0;
}

void DisplaySharp::SetContent(const DisplayBuffer &buffer)
{
    if (buffer.GetWidth() != width_ || buffer.GetHeight() != height_) {
        return;
    }

    SetChipSelected(1);
    CheckSpiMasterMode();

    SendByte(0b10000000 | GetVbyte()); //Write line command
    for (uint8_t y = 0; y < height_; ++y) {
        const uint8_t *data = buffer.GetBufferRow(y);

        // Command for line modification
        SendByte(BitFlip(y+1));

        for (uint8_t x = 0; x < width_/8; ++x) {
            SendByte(data[x]);
        }

        // 8 clocks of padding
        SendByte(0);
    }
    // Additional 8 clocks of padding
    SendByte(0);

    _delay_us(2);
    SetChipSelected(0);
}

void DisplaySharp::ToggleExtcomin()
{
    if (!vbit_) {
        vbit_ = true;
        if (NULL != excomin_port_) {
            excomin_port_->OUTSET = excomin_pin_;
        } else {
            SendVbyte();
        }
    } else {
        vbit_ = false;
        if (NULL != excomin_port_) {
            excomin_port_->OUTCLR = excomin_pin_;
        } else {
            SendVbyte();
        }
    }
}

void DisplaySharp::Clear()
{
    SetChipSelected(1);
    CheckSpiMasterMode();

    SendByte(0b00000100 | GetVbyte());
    SendByte(0);

    _delay_us(2);
    SetChipSelected(0);
}

void DisplaySharp::SetDisplayOn(const bool display_on)
{
    // Implement
    if (NULL != display_on_port_) {
        if (display_on) {
            display_on_port_->OUTSET = display_on_pin_;
        } else {
            display_on_port_->OUTCLR = display_on_pin_;
        }
    }
}

void DisplaySharp::SetChipSelected(const uint8_t val)
{
    if (val == 0) {
        cs_port_->OUTCLR = cs_pin_;
    } else {
        cs_port_->OUTSET = cs_pin_;
        _delay_us(6);
    }
}

void DisplaySharp::SendByte(const uint8_t val)
{
    if (reset_master_mode_automatically_) {
        SendByteMasterCheck(val);
    } else {
        SPI_SendByte(spi_, val);
    }
}

void DisplaySharp::SendByteMasterCheck(const uint8_t val)
{
    spi_->DATA = val;
    while (!(spi_->STATUS & SPI_IF_bm) && (spi_->CTRL & SPI_MASTER_bm));
}

uint8_t DisplaySharp::GetVbyte()
{
    // format 0x0V000000
    return vbit_ << 6;
}

void DisplaySharp::SendVbyte()
{
    SetChipSelected(true);
    CheckSpiMasterMode();
    SendByte(GetVbyte());
    SendByte(0); // Padding
    SetChipSelected(false);
}

void DisplaySharp::CheckSpiMasterMode()
{
    if (!reset_master_mode_automatically_) {
        return;
    }
    if ((SPI_GetCurrentMode(spi_) & SPI_MODE_MASTER) == 0) {
        // In slave mode. Reset
        spi_->CTRL |= SPI_MODE_MASTER;
    }
}
