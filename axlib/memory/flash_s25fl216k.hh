#ifndef AXLIB_MEMORY_FLASH_S25FL216K_HH
#define AXLIB_MEMORY_FLASH_S25FL216K_HH

#include "axlib/core/io.hh"

namespace axlib {

class FlashS25Fl216K
{
 public:
    FlashS25Fl216K(const Port spi_port,
                   const Port cs_port,
                   const Pin cs_pin);

    void ReadData(const uint8_t num_bytes, void *read_address, void *data);
 private:
    void SetChipSelected(const bool selected);
 private:
    SPI_t *spi_;
    PORT_t *spi_port_;
    PORT_t *cs_port_;
    uint8_t cs_pin_;
}; //class FlashS25Fl216K

} //namespace axlib

#endif //ifndef AXLIB_MEMORY_FLASH_S25FL216K_HH
