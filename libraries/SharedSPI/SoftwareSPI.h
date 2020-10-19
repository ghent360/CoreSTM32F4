#ifndef SOFTWARESPI_H
#define SOFTWARESPI_H

#include "Core.h"
#include "variant.h"
#include "SPI.h"

class SoftwareSPI: public SPI
{
public:
    SoftwareSPI();
    spi_status_t transceivePacket(const uint8_t *tx_data, uint8_t *rx_data, size_t len) NOEXCEPT;
    void configureDevice(uint32_t bits, uint32_t clockMode, uint32_t bitRate) NOEXCEPT; // Master mode
    void initPins(Pin clk, Pin miso, Pin mosi, Pin cs = NoPin, DMA_Stream_TypeDef* rxStream = nullptr, uint32_t rxChan = 0, IRQn_Type rxIrq = DMA1_Stream0_IRQn,
                            DMA_Stream_TypeDef* txStream = nullptr, uint32_t txChan = 0, IRQn_Type txIrq = DMA1_Stream0_IRQn) NOEXCEPT;
    bool waitForTxEmpty() NOEXCEPT;
    
    static SoftwareSPI SWSSP0;

private:
    
    uint8_t transfer_byte(uint8_t byte_out) NOEXCEPT;
    
    bool needInit;
    Pin sck;
    Pin mosi;
    Pin miso;
};




#endif
