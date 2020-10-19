#ifndef HARDWARESPI_H
#define HARDWARESPI_H

#include "Core.h"
#include "SPI.h"

#include "FreeRTOS.h"
#include "task.h"
#include "spi_com.h"
extern "C" void DMA2_Stream2_IRQHandler(void) NOEXCEPT;
extern "C" void DMA2_Stream3_IRQHandler(void) NOEXCEPT;
extern "C" void DMA1_Stream3_IRQHandler(void) NOEXCEPT;
extern "C" void DMA1_Stream4_IRQHandler(void) NOEXCEPT;

class HardwareSPI;
typedef void (*SPICallbackFunction)(HardwareSPI *spiDevice) NOEXCEPT;
class HardwareSPI: public SPI
{
public:
    HardwareSPI() NOEXCEPT;
    spi_status_t transceivePacket(const uint8_t *tx_data, uint8_t *rx_data, size_t len) NOEXCEPT;
    bool waitForTxEmpty() NOEXCEPT;
    void configureDevice(uint32_t bits, uint32_t clockMode, uint32_t bitRate) NOEXCEPT; // Master mode
    void configureDevice(uint32_t deviceMode, uint32_t bits, uint32_t clockMode, uint32_t bitRate, bool hardwareCS) NOEXCEPT;
    void initPins(Pin clk, Pin miso, Pin mosi, Pin cs = NoPin, DMA_Stream_TypeDef* rxStream = nullptr, uint32_t rxChan = 0, IRQn_Type rxIrq = DMA1_Stream0_IRQn,
                            DMA_Stream_TypeDef* txStream = nullptr, uint32_t txChan = 0, IRQn_Type txIrq = DMA1_Stream0_IRQn) NOEXCEPT;
    void disable() NOEXCEPT;
    void flushRx() NOEXCEPT;
    void startTransfer(const uint8_t *tx_data, uint8_t *rx_data, size_t len, SPICallbackFunction ioComplete) NOEXCEPT;
    void stopTransfer() NOEXCEPT;
    static HardwareSPI SSP1;
    static HardwareSPI SSP2;
    static HardwareSPI SSP3;

private:
    bool usingDma;
    spi_t spi;
    DMA_HandleTypeDef dmaRx;
    DMA_HandleTypeDef dmaTx;
    Pin csPin;
    uint32_t curBitRate;
    uint32_t curBits;
    uint32_t curClockMode;
    bool initComplete;
    SPICallbackFunction callback;
    TaskHandle_t waitingTask;

    void initDmaStream(DMA_HandleTypeDef& hdma, DMA_Stream_TypeDef *inst, uint32_t chan, IRQn_Type irq, uint32_t dir, uint32_t minc) NOEXCEPT;
    void startTransferAndWait(const uint8_t *tx_data, uint8_t *rx_data, size_t len) NOEXCEPT;

    friend void DMA2_Stream2_IRQHandler() NOEXCEPT;
    friend void DMA2_Stream3_IRQHandler() NOEXCEPT;
    friend void DMA1_Stream3_IRQHandler() NOEXCEPT;
    friend void DMA1_Stream4_IRQHandler() NOEXCEPT;
    friend void transferComplete(HardwareSPI *spiDevice) NOEXCEPT;
    friend void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
    friend void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
};

#endif
