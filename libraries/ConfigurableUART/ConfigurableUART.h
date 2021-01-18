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
	typedef void (*InterruptCallbackFn)(ConfigurableUART*) NOEXCEPT;
	struct Errors
	{
		uint32_t uartOverrun,
				 framing,
				 bufferOverrun;

		Errors() NOEXCEPT : uartOverrun(0), framing(0), bufferOverrun(0)  {  }
	};

    ConfigurableUART() NOEXCEPT;

    bool Configure(uint32_t rx, uint32_t tx) NOEXCEPT;
    
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

    bool IsConnected() NOEXCEPT;

    int8_t GetUARTPortNumber() NOEXCEPT;

    void setInterruptPriority(uint32_t priority) NOEXCEPT;
    uint32_t getInterruptPriority() NOEXCEPT;

    InterruptCallbackFn SetInterruptCallback(InterruptCallbackFn f) NOEXCEPT;

	// Get and clear the errors
	Errors GetAndClearErrors() NOEXCEPT;

private:
    HardwareSerial *serialPort;
    InterruptCallbackFn interruptCallback;
    Errors errors;
};

#define UARTClass ConfigurableUART // compatibility with RRF

extern ConfigurableUART UART_Slot0;
extern ConfigurableUART UART_Slot1;
extern ConfigurableUART UART_Slot2;


#endif
