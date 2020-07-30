//Hardware SPI
#include "HardwareSPI.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "spi_com.h"

// Create SPI devices with default pins, these may get changed to alternates later
HardwareSPI HardwareSPI::SSP1;
HardwareSPI HardwareSPI::SSP2;
HardwareSPI HardwareSPI::SSP3;
//HardwareSPI HardwareSPI::SSP2(PB_13, PB_14, PB_15, PB_12);
//HardwareSPI HardwareSPI::SSP3(PC_10, PC_11, PC_12, PA_15);

//#define SSPI_DEBUG
extern "C" void debugPrintf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));

static inline void flushTxFifo(SPI_HandleTypeDef *sspDevice) noexcept
{

}

static inline void flushRxFifo(SPI_HandleTypeDef *hspi) noexcept
{
    uint8_t dummy;
    int cnt = 0;
    while (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
    {
        /* read the received data */
        dummy = *(__IO uint8_t *)&hspi->Instance->DR;
        cnt++;
    }
    //if (cnt > 0)
        //debugPrintf("Flushed cnt %d\n", cnt);

}

void HardwareSPI::flushRx() noexcept
{
    flushRxFifo(&spi.handle);
}

// Disable the device and flush any data from the fifos
void HardwareSPI::disable() noexcept
{
    debugPrintf("spi disable called\n");
    if (initComplete)
    {
        HAL_SPI_DMAStop(&(spi.handle));
        flushRxFifo(&spi.handle);
        spi_deinit(&spi);
        initComplete = false;
    }
}

// Wait for transmitter empty returning true if timed out
//static inline bool waitForTxEmpty(LPC_SSP_TypeDef* sspDevice)
bool HardwareSPI::waitForTxEmpty() noexcept
{
    return false;
}

static inline uint32_t getSSPBits(uint8_t bits) noexcept
{

    return 8;
}


static inline uint32_t getSSPMode(uint8_t spiMode) noexcept
{
    return 0;
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) noexcept
{
    // Get pointer to containing object
    HardwareSPI *s = (HardwareSPI *)((uint8_t *)hspi - ((uint8_t *)&(HardwareSPI::SSP1.spi.handle) - (uint8_t *)&HardwareSPI::SSP1));
    if (s->callback) s->callback(s);    
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) noexcept
{
    // Get pointer to containing object 
    HardwareSPI *s = (HardwareSPI *)((uint8_t *)hspi - ((uint8_t *)&(HardwareSPI::SSP1.spi.handle) - (uint8_t *)&HardwareSPI::SSP1));
    if (s->callback) s->callback(s);    
}    

extern "C" void DMA2_Stream2_IRQHandler()
{
    HAL_DMA_IRQHandler(&(HardwareSPI::SSP1.dmaRx));    
}

extern "C" void DMA2_Stream3_IRQHandler()
{
    HAL_DMA_IRQHandler(&(HardwareSPI::SSP1.dmaTx));    
}

extern "C" void DMA1_Stream3_IRQHandler()
{
    HAL_DMA_IRQHandler(&(HardwareSPI::SSP2.dmaRx));    
}

extern "C" void DMA1_Stream4_IRQHandler()
{
    HAL_DMA_IRQHandler(&(HardwareSPI::SSP2.dmaTx));    
}

// Called on completion of a blocking transfer
void transferComplete(HardwareSPI *spiDevice) noexcept
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(spiDevice->waitingTask, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void HardwareSPI::initPins(Pin clk, Pin miso, Pin mosi, Pin cs, DMA_Stream_TypeDef* rxStream, uint32_t rxChan, IRQn_Type rxIrq,
                            DMA_Stream_TypeDef* txStream, uint32_t txChan, IRQn_Type txIrq) noexcept
{
    spi.pin_sclk = clk;
    spi.pin_miso = miso;
    spi.pin_mosi = mosi;
    spi.pin_ssel = csPin = cs;   
    // init the DMA channels
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    initDmaStream(dmaRx, rxStream, rxChan, rxIrq, DMA_PERIPH_TO_MEMORY, DMA_MINC_ENABLE);
    initDmaStream(dmaTx, txStream, txChan, txIrq, DMA_MEMORY_TO_PERIPH, DMA_MINC_ENABLE);
    __HAL_LINKDMA(&(spi.handle), hdmarx, dmaRx);
    __HAL_LINKDMA(&(spi.handle), hdmatx, dmaTx);  
    initComplete = false;
}

void HardwareSPI::initDmaStream(DMA_HandleTypeDef& hdma, DMA_Stream_TypeDef *inst, uint32_t chan, IRQn_Type irq, uint32_t dir, uint32_t minc) noexcept
{
    hdma.Instance                 = inst;
    
    hdma.Init.Channel             = chan;
    hdma.Init.Direction           = dir;
    hdma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma.Init.MemInc              = minc;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma.Init.Mode                = DMA_NORMAL;
    hdma.Init.Priority            = DMA_PRIORITY_LOW;
    hdma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
    hdma.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    
    HAL_DMA_Init(&hdma); 
    NVIC_EnableIRQ(irq);      
}

void HardwareSPI::configureDevice(uint32_t deviceMode, uint32_t bits, uint32_t clockMode, uint32_t bitRate, bool hardwareCS) noexcept
{
    Pin cs = (hardwareCS ? csPin : NoPin);
    if (!initComplete || bitRate != curBitRate || bits != curBits || clockMode != curClockMode )
    {
        if (initComplete)
        {
            HAL_SPI_DMAStop(&(spi.handle));
            spi_deinit(&spi);
        }
        spi.pin_ssel = cs;
        spi_init(&spi, deviceMode, bitRate, (spi_mode_e)clockMode, 1);
        initComplete = true;
        curBitRate = bitRate;
        curBits = bits;
        curClockMode = clockMode;
    }
}

//setup the master device.
void HardwareSPI::configureDevice(uint32_t bits, uint32_t clockMode, uint32_t bitRate) noexcept
{
    configureDevice(SPI_MODE_MASTER, bits, clockMode, bitRate, false);
}


HardwareSPI::HardwareSPI() noexcept :initComplete(false)
{
    curBitRate = 0xffffffff;
    curClockMode = 0xffffffff;
    curBits = 0xffffffff;
}

void HardwareSPI::checkComplete() noexcept
{
    HAL_SPI_StateTypeDef state = HAL_SPI_GetState(&(spi.handle));
    if (state != HAL_SPI_STATE_READY)
    {
        debugPrintf("SPI not ready %x\n", state);
    }
    HAL_DMA_StateTypeDef dmaState = HAL_DMA_GetState(spi.handle.hdmarx);
    if (dmaState != HAL_DMA_STATE_READY)
    {
        debugPrintf("RX DMA not ready %x\n", dmaState);
    }
    dmaState = HAL_DMA_GetState(spi.handle.hdmatx);
    if (dmaState != HAL_DMA_STATE_READY)
    {
        debugPrintf("TX DMA not ready %x\n", dmaState);
    }
    uint32_t err = HAL_DMA_GetError(spi.handle.hdmarx);
    if (err != 0)
        debugPrintf("rx dma error %d\n", err);
    err = HAL_DMA_GetError(spi.handle.hdmatx);
    if (err != 0)
        debugPrintf("tx dma error %d\n", err);
}

void HardwareSPI::startTransfer(const uint8_t *tx_data, uint8_t *rx_data, size_t len, SPICallbackFunction ioComplete) noexcept
{
    // FIXME using DMA for a small number of bytes is probably a very bad idea, there is a lot of setup
    // consider turning off meminc when rx_data or tx_data is null so we don't need to memset buffers to 0xff for the sd card.
    // consider setting dms burst size to 4 for WiFi and SBC transfers
    HAL_SPI_StateTypeDef state = HAL_SPI_GetState(&(spi.handle));
    if (state != HAL_SPI_STATE_READY)
    {
        debugPrintf("SPI not ready %x\n", state);
        delay(100);
    }
    HAL_DMA_StateTypeDef dmaState = HAL_DMA_GetState(spi.handle.hdmarx);
    if (dmaState != HAL_DMA_STATE_READY)
    {
        debugPrintf("RX DMA not ready %x\n", dmaState);
        delay(100);
    }
    dmaState = HAL_DMA_GetState(spi.handle.hdmatx);
    if (dmaState != HAL_DMA_STATE_READY)
    {
        debugPrintf("TX DMA not ready %x\n", dmaState);
        delay(100);
    }

    HAL_StatusTypeDef status;    
    callback = ioComplete;
    if (rx_data == nullptr)
        status = HAL_SPI_Transmit_DMA(&(spi.handle), (uint8_t *)tx_data, len);
    else if (tx_data == nullptr)
        status = HAL_SPI_TransmitReceive_DMA(&(spi.handle), rx_data, rx_data, len);
    else
        status = HAL_SPI_TransmitReceive_DMA(&(spi.handle), (uint8_t *)tx_data, rx_data, len);
    if (status != HAL_OK)
        debugPrintf("SPI Error %d\n", (int)status);
}

spi_status_t HardwareSPI::transceivePacket(const uint8_t *tx_data, uint8_t *rx_data, size_t len) noexcept
{
    waitingTask = xTaskGetCurrentTaskHandle();
    startTransfer(tx_data, rx_data, len, transferComplete);
    spi_status_t ret = SPI_OK;
    const TickType_t xDelay = SPITimeoutMillis / portTICK_PERIOD_MS; //timeout
    if( ulTaskNotifyTake(pdTRUE, xDelay) == 0) // timed out
    {
        ret = SPI_TIMEOUT;
        debugPrintf("SPI timeout\n");
    }
    return ret;
}
