//Author: sdavi

#ifndef _CONFIGURABLEUART_H_
#define _CONFIGURABLEUART_H_


#include "Core.h"
#include "HardwareSerial.h"
#include "Stream.h"
#include "Print.h"




class ConfigurableUART : public Stream
{

public:
    ConfigurableUART() NOEXCEPT;

    bool Configure(Pin rx, Pin tx) NOEXCEPT;
    
    void begin(uint32_t baud) NOEXCEPT;
    void end() NOEXCEPT;

    size_t write(const uint8_t *buffer, size_t size) NOEXCEPT override;
    size_t write(uint8_t) NOEXCEPT override;

    int available(void) NOEXCEPT;
    int availableForWrite(void) NOEXCEPT;
    
    int peek(void) NOEXCEPT;
    int read(void) NOEXCEPT;
    void flush(void) NOEXCEPT;
    using Print::write;
    size_t canWrite() NOEXCEPT;

    bool IsConnected() NOEXCEPT;

    int8_t GetUARTPortNumber() NOEXCEPT;

    void setInterruptPriority(uint32_t priority) NOEXCEPT;
    uint32_t getInterruptPriority() NOEXCEPT;

private:
    HardwareSerial *serialPort;
};

#define UARTClass ConfigurableUART // compatibility with RRF

extern ConfigurableUART UART_Slot0;
extern ConfigurableUART UART_Slot1;
extern ConfigurableUART UART_Slot2;


#endif
