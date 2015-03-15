#ifndef AXLIB_MEMORY_FLASH_S25FL216K_HH
#define AXLIB_MEMORY_FLASH_S25FL216K_HH

#include "axlib/core/io.hh"
#include <stdint.h>

namespace axlib {

class FlashS25Fl216K
{
 public:
    FlashS25Fl216K(const Port spi_port,
                   const Port cs_port,
                   const Pin cs_pin);

    void ReadData(uint8_t *data, uint32_t read_address, const uint32_t num_bytes);

    /// Write data to flash. Can write more than 256 bytes safely
    /// \note The pages which are written must have been erased before the write
    void WriteData(uint32_t write_address, uint8_t *data, const uint32_t num_bytes);
    void Erase4k(uint32_t write_address);
    uint8_t ReadStatus();
    void WritePage256(uint32_t write_address, uint8_t *data, const uint8_t num_bytes);
    static uint32_t GetNext256Page(uint32_t address);
    static uint32_t GetPage256Begin(uint32_t address);
    static uint32_t GetSector4kBegin(uint32_t address);
 private:
    void WriteEnable();
    void SetChipSelected(const bool selected);
    void InitSpi();
    void WaitUntilReady();


 private:
    SPI_t *spi_;
    PORT_t *spi_port_;
    PORT_t *cs_port_;
    uint8_t cs_pin_;
}; //class FlashS25Fl216K

} //namespace axlib

#endif //ifndef AXLIB_MEMORY_FLASH_S25FL216K_HH
