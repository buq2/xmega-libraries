#ifndef THERMO_MAX6675_HH
#define THERMO_MAX6675_HH

#include <stdint.h>
#include "axlib/core/io.hh"

namespace axlib {

class ThermoMax6675
{
 public:
    /// \param[in] spi_port Port which is used as SPI
    /// \param[in] cs_port Port which contains the CS pin for the chip
    /// \param[in] cs_pin Pin of cs_port which used for CS
    ThermoMax6675(const axlib::Port spi_port, const axlib::Port cs_port,
                 const uint8_t cs_pin);

    /// \return True if success
    /// \note Must not be called faster than one in 220ms as
    ///     reading of the value will stop AD conversion
    bool GetTemperatureCelcius(uint16_t *temperature);

    /// Set bias of the system. This value will be removed from the returned
    /// temperature values
    /// \param[in] bias Bias of the system
    void SetTemperatureBiasCelcius(const uint16_t bias);
 private:
    /// \param[in] selected If true, chip will be selected
    void SetChipSelected(const bool selected);

    /// Initialize hardware
    bool Setup();
 private:
    // Port related
    PORT_t *spi_port_;
    SPI_t *spi_;
    PORT_t *cs_port_;
    uint8_t cs_pin_;

    // Temperature correction
    uint16_t temperature_bias_;
}; //class ThermoMax6675

} //class axlib

#endif //ifndef THERMO_MAX6675_HH
