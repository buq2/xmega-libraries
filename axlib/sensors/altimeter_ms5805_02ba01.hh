#ifndef ALTIMETER_MS5805_02BA01_HH
#define ALTIMETER_MS5805_02BA01_HH

#include "axlib/core/io.hh"
#include <stdint.h>

namespace axlib {

class AltimeterMS5805_02BA01
{
 public:
    AltimeterMS5805_02BA01(const Port i2c_port);
    uint8_t Setup();

    uint8_t GetAltitudeMeters(float *altitude);
    uint8_t GetTemperatureCelcius(float *temp);

    typedef enum
    {
        OversampleRatio256 = 0,
        OversampleRatio512 = 1,
        OversampleRatio1024 = 2,
        OversampleRatio2048 = 3,
        OversampleRatio4096 = 4,
        OversampleRatio8192 = 5
    } OversampleRatio;

    typedef enum
    {
        RawValueD1,
        RawValueD2
    } RawValue;

    void SetOversampleRatio(const OversampleRatio ratio);
    void GetStr(char *str);
 private:
    uint8_t ReadCoefficient(const uint8_t coefnum, uint16_t *coef);
    uint8_t ReadMeasurement(const RawValue d, const OversampleRatio ratio, uint32_t *value);
 private:
    uint16_t coefficients_[8];
    OversampleRatio ratio_;
    TWI_t *i2c_port_;
}; //class AltimeterMS5805_02BA01

} //namespace axlib

#endif //ifndef ALTIMETER_MS5805_02BA01_HH
