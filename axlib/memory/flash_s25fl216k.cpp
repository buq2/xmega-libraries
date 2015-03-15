#include "axlib/memory/flash_s25fl216k.hh"
#include "axlib/core/core.hh"
#include <LUFA/Drivers/Peripheral/SPI.h>

using namespace axlib;

#define SPI_MOSI_PIN 0b00100000
#define SPI_SCK_PIN 0b10000000

FlashS25Fl216K::FlashS25Fl216K(const Port spi_port, const Port cs_port, const Pin cs_pin)
    :
      spi_(GetSpiPort(spi_port)),
      spi_port_(GetPort(spi_port)),
      cs_port_(GetPort(cs_port)),
      cs_pin_((uint8_t)cs_pin)
{
    // Setup SPI pins
    spi_port_->DIRSET  = SPI_MOSI_PIN | SPI_SCK_PIN;

    // Set the actual CS pin (which is not in the SPI port)
    cs_port_->DIRSET = cs_pin_;
    cs_port_->OUTCLR = cs_pin_;

    InitSpi();

    SetChipSelected(false);
}

void FlashS25Fl216K::WriteEnable()
{
    SetChipSelected(true);
    SendByte(0x06);
    SetChipSelected(false);
}

void FlashS25Fl216K::Erase4k(uint32_t write_address)
{
    WaitUntilReady();
    WriteEnable();
    SetChipSelected(true);
    SendByte(0x20);
    SendByte((write_address>>8*2) & 0xFF);
    SendByte((write_address>>8*1) & 0xFF);
    SendByte((write_address>>8*0) & 0xFF);
    SetChipSelected(false);
}

void FlashS25Fl216K::FullErase()
{
    WaitUntilReady();
    WriteEnable();
    SetChipSelected(true);
    SendByte(0xc7);
    SetChipSelected(false);
}

uint8_t FlashS25Fl216K::ReadStatus()
{
    SetChipSelected(true);
    SendByte(0x05);
    const uint8_t status = SPI_ReceiveByte_MasterCheck(spi_);
    SetChipSelected(false);
    return status;
}

void FlashS25Fl216K::SetChipSelected(const bool selected)
{
    if (selected) {
        InitSpi();
        cs_port_->OUTCLR = cs_pin_;
    } else {
        cs_port_->OUTSET = cs_pin_;
    }
}

void FlashS25Fl216K::InitSpi()
{
    SPI_Init(spi_,
             SPI_SPEED_FCPU_DIV_2  | SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_RISING |
             SPI_SAMPLE_LEADING | SPI_MODE_MASTER);
}

void FlashS25Fl216K::SendByte(const uint8_t byte)
{
    SPI_SendByte_MasterCheck(spi_, byte);
}

uint8_t FlashS25Fl216K::ReceiveByte()
{
    return SPI_ReceiveByte_MasterCheck(spi_);
}

void FlashS25Fl216K::WaitUntilReady()
{
    while (ReadStatus() & 0b00000001) {
    }
}

uint32_t FlashS25Fl216K::GetNext256Page(uint32_t address)
{
    return (address & 0xffff00) + 0x000100;
}

uint32_t FlashS25Fl216K::GetNext4kSector(uint32_t address)
{
    uint32_t next = GetSector4kBegin(address);
    return next+4096;
}

uint32_t FlashS25Fl216K::GetPage256Begin(uint32_t address)
{
    return address & 0xffff00;
}

uint32_t FlashS25Fl216K::GetSector4kBegin(uint32_t address)
{
    return address & 0xfff000;
}


void FlashS25Fl216K::ReadData(uint8_t *data, uint32_t read_address, const uint32_t num_bytes)
{
    WaitUntilReady();
    SetChipSelected(true);
    SendByte(0x03);

    // Send read address
    SendByte((read_address>>8*2) & 0xFF);
    SendByte((read_address>>8*1) & 0xFF);
    SendByte((read_address>>8*0) & 0xFF);

    for (unsigned int i = 0; i < num_bytes; ++i) {
        data[i] = ReceiveByte();
    }

    SetChipSelected(false);
}

void FlashS25Fl216K::WriteData(uint32_t write_address, const uint8_t *data_in, const uint32_t num_bytes)
{
    uint8_t *data = const_cast<uint8_t*>(data_in);
    uint32_t bytes_left = num_bytes;
    uint32_t address = write_address;
    while(bytes_left) {
        uint32_t bytes_left_in_page = GetNext256Page(address)-write_address;
        uint32_t bytes_to_write = axlib::min(bytes_left_in_page,bytes_left);
        WritePage256(address,data,bytes_to_write);
        data += bytes_to_write;
        address += bytes_to_write;
        bytes_left -= bytes_to_write;
    }

}

void FlashS25Fl216K::WritePage256(uint32_t write_address, uint8_t *data, const uint8_t num_bytes)
{
    WaitUntilReady();
    WriteEnable();
    SetChipSelected(true);
    SendByte(0x02);

    // Send write address
    SendByte((write_address>>8*2) & 0xFF);
    SendByte((write_address>>8*1) & 0xFF);
    SendByte((write_address>>8*0) & 0xFF);

    for (unsigned int i = 0; i < num_bytes; ++i) {
        SendByte(data[i]);
    }

    SetChipSelected(false);
}
