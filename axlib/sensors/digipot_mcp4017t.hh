#ifndef AXLIB_SENSORS_DIGIPOT_MCP4017T_HH
#define AXLIB_SENSORS_DIGIPOT_MCP4017T_HH

#include "axlib/core/io.hh"

namespace axlib {

class DigipotMcp4017T
{
 public:
    DigipotMcp4017T(const Port i2c_port);
    uint8_t SetValue(const uint8_t val);
    uint8_t Setup();
private:
    TWI_t *i2c_port_;
}; //class Digipot

} //namespace axlib

#endif //ifndef AXLIB_SENSORS_DIGIPOT_MCP4017T_HH
