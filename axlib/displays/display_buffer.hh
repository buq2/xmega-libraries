#ifndef DISPLAY_BUFFER_HH
#define DISPLAY_BUFFER_HH

#include <stdint.h>
#include "axlib/displays/fonts/fontlibrary.h"

namespace axlib {

class DisplayBuffer
{
 public:
    typedef enum
    {
        ROTATION_NONE = 0,
        ROTATION_90 = 1,
        ROTATION_180 = 2,
        ROTATION_270 = 3,
        ROTATION_NUM_ENUMS = 4
    } Rotation;

    DisplayBuffer(const uint8_t width, const uint8_t height);
    ~DisplayBuffer();

    const uint8_t *GetBuffer() const;
    const uint8_t *GetBufferRow(const uint8_t &row) const;
    uint16_t GetNumberOfBytes() const;
    uint8_t GetWidth() const;
    uint8_t GetHeight() const;
    void RenderText(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str);
    void RenderText_AlignRight(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str);
    void RenderText_AlignCenter(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str);
    static uint8_t CalculateTextWidthPixels(const fontStyle_t &font, const uint8_t scale_x, const char *str);
    static uint8_t CalculateTextHeightPixels(const fontStyle_t &font, const uint8_t scale_y, const char *str);
    static uint8_t CalculateTextHeightPixels(const fontStyle_t &font, const uint8_t scale_y);
    typedef enum PixelManipulate_t {
        PIXEL_SET,
        PIXEL_CLEAR,
        PIXEL_TOGGLE
    } PixelManipulate;
    void ModifyPixel(const uint8_t x, const uint8_t y, const PixelManipulate op);
    void ModifyBit(const uint8_t x, const uint8_t y, const PixelManipulate op);
    bool GetPixel(const uint8_t x, const uint8_t y);
    void BlitGraphics(const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y,
                      const uint8_t width_bits, const uint8_t stride_bytes, const uint8_t height_rows,
                      const uint8_t *data);
    void BlitRow(const uint8_t x, const uint8_t y, const uint8_t *data, const uint8_t width_bits);
    void BlitRow(const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t *data, const uint8_t width_bits);
    void Clear();
    void SetRotation(const Rotation rot);
    void SetInvertColors(const bool invert);
 private:
    uint8_t width_;
    uint8_t height_;
    uint8_t stride_;
    uint8_t data_[128*128/8];
    Rotation rotation_;
    bool invert_colors_;
}; //class DisplayBuffer

} //namespace axlib

#endif //ifndef DISPLAY_BUFFER_HH
