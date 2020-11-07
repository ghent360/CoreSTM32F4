/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

#ifndef TwoWire_h
#define TwoWire_h

#include "Stream.h"
#include "Arduino.h"
extern "C" {
#include "utility/twi.h"
}

#define BUFFER_LENGTH 32

#define MASTER_ADDRESS 0x33

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

class TwoWire : public Stream {
  private:
    uint8_t *rxBuffer;
    uint8_t rxBufferAllocated;
    uint8_t rxBufferIndex;
    uint8_t rxBufferLength;

    uint8_t txAddress;
    uint8_t *txBuffer;
    uint8_t txBufferAllocated;
    uint8_t txBufferIndex;
    uint8_t txBufferLength;

    uint8_t transmitting;

    uint8_t ownAddress;
    i2c_t _i2c;

    void (*user_onRequest)(void);
    void (*user_onReceive)(int);
    static void onRequestService(i2c_t *) NOEXCEPT;
    static void onReceiveService(i2c_t *) NOEXCEPT;

    void allocateRxBuffer(size_t length) NOEXCEPT;
    void allocateTxBuffer(size_t length) NOEXCEPT;

    void resetRxBuffer(void) NOEXCEPT;
    void resetTxBuffer(void) NOEXCEPT;

  public:
    TwoWire() NOEXCEPT;
    TwoWire(uint8_t sda, uint8_t scl) NOEXCEPT;
    // setSCL/SDA have to be called before begin()
    void setSCL(uint32_t scl) NOEXCEPT
    {
      _i2c.scl = digitalPinToPinName(scl);
    };
    void setSDA(uint32_t sda) NOEXCEPT
    {
      _i2c.sda = digitalPinToPinName(sda);
    };
    void setSCL(PinName scl) NOEXCEPT
    {
      _i2c.scl = scl;
    };
    void setSDA(PinName sda) NOEXCEPT
    {
      _i2c.sda = sda;
    };
    void begin(bool generalCall = false) NOEXCEPT;
    void begin(uint8_t, uint8_t) NOEXCEPT;
    void begin(uint8_t, bool generalCall = false) NOEXCEPT;
    void begin(int, bool generalCall = false) NOEXCEPT;
    void end() NOEXCEPT;
    void setClock(uint32_t) NOEXCEPT;
    void beginTransmission(uint8_t) NOEXCEPT;
    void beginTransmission(int) NOEXCEPT;
    uint8_t endTransmission(void) NOEXCEPT;
    uint8_t endTransmission(uint8_t) NOEXCEPT;
    uint8_t requestFrom(uint8_t, uint8_t) NOEXCEPT;
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t) NOEXCEPT;
    uint8_t requestFrom(uint8_t, size_t, bool) NOEXCEPT;
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t) NOEXCEPT;
    uint8_t requestFrom(int, int) NOEXCEPT;
    uint8_t requestFrom(int, int, int) NOEXCEPT;
    virtual size_t write(uint8_t) NOEXCEPT;
    virtual size_t write(const uint8_t *, size_t) NOEXCEPT;
    virtual int available(void) NOEXCEPT;
    virtual int read(void) NOEXCEPT;
    virtual int peek(void) NOEXCEPT;
    virtual void flush(void) NOEXCEPT;
    void onReceive(void (*)(int) NOEXCEPT) NOEXCEPT;
    void onRequest(void (*)(void) NOEXCEPT) NOEXCEPT;

    inline size_t write(unsigned long n) NOEXCEPT
    {
      return write((uint8_t)n);
    }
    inline size_t write(long n) NOEXCEPT
    {
      return write((uint8_t)n);
    }
    inline size_t write(unsigned int n) NOEXCEPT
    {
      return write((uint8_t)n);
    }
    inline size_t write(int n) NOEXCEPT
    {
      return write((uint8_t)n);
    }
    using Print::write;
};



extern TwoWire Wire;

#endif
