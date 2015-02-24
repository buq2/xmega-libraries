#ifndef DISPLAY_SHARP_HH
#define DISPLAY_SHARP_HH

#include <stdint.h>
#include "axlib/displays/display_buffer.hh"

namespace axlib {

class DisplaySharp
{
 public:
    DisplaySharp(const uint8_t width, const uint8_t height);
    bool Setup();
    void SetContent(const DisplayBuffer &buffer);
    void ToggleExtcomin();
    void Clear();
 private:
    void SetChipSelected(const uint8_t val);
    void SendByte(const uint8_t val);
 private:
    uint8_t width_;
    uint8_t height_;
}; //class DisplaySharp

} //namespace axlib

#endif //ifndef DISPLAY_SHARP_HH
