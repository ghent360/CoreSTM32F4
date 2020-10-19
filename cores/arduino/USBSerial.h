/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _USBSERIAL_H_
#define _USBSERIAL_H_

#if defined (USBCON) && defined(USBD_USE_CDC)
#include "Stream.h"
#include "usbd_core.h"

//================================================================================
// Serial over CDC
class USBSerial : public Stream {
  public:
    void begin(void) NOEXCEPT;
    void begin(uint32_t) NOEXCEPT;
    void begin(uint32_t, uint8_t) NOEXCEPT;
    void end(void) NOEXCEPT;

    virtual int available(void) NOEXCEPT;
    virtual int availableForWrite(void) NOEXCEPT;
    virtual int peek(void) NOEXCEPT;
    virtual int read(void) NOEXCEPT;
    virtual size_t readBytes(char *buffer, size_t length) NOEXCEPT;  // read chars from stream into buffer
    virtual size_t readBytesUntil(char terminator, char *buffer, size_t length) NOEXCEPT;  // as readBytes with terminator character
    virtual void flush(void) NOEXCEPT;
    virtual size_t write(uint8_t) NOEXCEPT;
    virtual size_t write(const uint8_t *buffer, size_t size) NOEXCEPT;
    using Print::write; // pull in write(str) from Print
    operator bool(void) NOEXCEPT;
    bool IsConnected() NOEXCEPT;

    // These return the settings specified by the USB host for the
    // serial port. These aren't really used, but are offered here
    // in case a sketch wants to act on these settings.
    uint32_t baud() NOEXCEPT;
    uint8_t stopbits() NOEXCEPT;
    uint8_t paritytype() NOEXCEPT;
    uint8_t numbits() NOEXCEPT;
    bool dtr() NOEXCEPT;
    bool rts() NOEXCEPT;
    enum {
      ONE_STOP_BIT = 0,
      ONE_AND_HALF_STOP_BIT = 1,
      TWO_STOP_BITS = 2,
    };
    enum {
      NO_PARITY = 0,
      ODD_PARITY = 1,
      EVEN_PARITY = 2,
      MARK_PARITY = 3,
      SPACE_PARITY = 4,
    };
};

extern USBSerial SerialUSB;
#endif /* USBCON */
#endif /* _USBSERIAL_H_ */
