#ifndef DISPLAY_SHARP_HH
#define DISPLAY_SHARP_HH

#include <stdint.h>
#include "axlib/displays/display_buffer.hh"
#include "axlib/core/io.hh"

namespace axlib {

class DisplaySharp
{
 public:
    DisplaySharp(const uint8_t width, const uint8_t height,
                 const Port spi_port,
                 const Port cs_port, const uint8_t cs_pin,
                 const Port excomin_port, const uint8_t excomin_pin,
                 const Port display_on_port, const uint8_t display_on_pin);
    bool Setup();
    void SetContent(const DisplayBuffer &buffer);
    void ToggleExtcomin();
    void Clear();
    void SetDisplayOn(const bool display_on);
 private:
    void SetChipSelected(const uint8_t val);
    void SendByte(const uint8_t val);

    /// This method will wait for the byte to be sent until
    /// the byte is sent, or we are no longer in master mode
    /// This function was created because SS pin was used as a
    /// button which pulled the SS pin to low when pressed.
    /// This causes SPI to become slave if SS pin is in input mode
    /// -> byte would never be sent and we would get stuck in
    /// normal mode.
    /// Will be called if reset_master_mode_automatically_ is true
    void SendByteMasterCheck(const uint8_t val);
    uint8_t GetVbyte();
    void SendVbyte();
    void CheckSpiMasterMode();
 private:
    uint8_t width_;
    uint8_t height_;

    // Port related
    PORT_t *spi_port_;
    SPI_t *spi_;
    PORT_t *cs_port_;
    uint8_t cs_pin_;
    PORT_t *excomin_port_;
    uint8_t excomin_pin_;
    PORT_t *display_on_port_;
    uint8_t display_on_pin_;

    // Vbit is used to change the polarity
    // it is embedded in every command
    bool vbit_;

    // If true, master spi mode is automatically reset
    // if it "slips" into slave mode. Also changes how bytes
    // are sent
    bool reset_master_mode_automatically_;
}; //class DisplaySharp

} //namespace axlib

#endif //ifndef DISPLAY_SHARP_HH
