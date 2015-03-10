#ifndef AXLIB_SENSORS_CLOCK_MCP7940M_HH
#define AXLIB_SENSORS_CLOCK_MCP7940M_HH

#include "axlib/core/io.hh"

namespace axlib {

class ClockMcp7940M
{
 public:
    ClockMcp7940M(const Port i2c_port);
    uint8_t Setup();
    uint8_t ReadSeconds(uint8_t *seconds);
    uint8_t ReadMinutes(uint8_t *minutes);
    uint8_t ReadHours(uint8_t *hours);
    uint8_t ReadDay(uint8_t *day);
    uint8_t ReadMonth(uint8_t *month);
    uint8_t ReadYear(uint8_t *year);
    uint8_t SetDate(const uint8_t year, const uint8_t month, const uint8_t day);
    char *GetTimeString();
 private:
 private:
    TWI_t *i2c_port_;
    char time_string_[32];
}; //class ClockMcp7940M

} //namespace axlib

#endif //ifndef AXLIB_SENSORS_CLOCK_MCP7940M_HH
