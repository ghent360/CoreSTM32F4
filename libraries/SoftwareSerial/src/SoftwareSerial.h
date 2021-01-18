/*
 * SoftwareSerial.h (formerly NewSoftSerial.h)
 *
 * Multi-instance software serial library for Arduino/Wiring
 * -- Interrupt-driven receive and other improvements by ladyada
 *    (http://ladyada.net)
 * -- Tuning, circular buffer, derivation from class Print/Stream,
 *    multi-instance support, porting to 8MHz processors,
 *    various optimizations, PROGMEM delay tables, inverse logic and
 *    direct port writing by Mikal Hart (http://www.arduiniana.org)
 * -- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
 * -- 20MHz processor support by Garrett Mace (http://www.macetech.com)
 * -- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * The latest version of this library can always be found at
 * http://arduiniana.org.
 */

#ifndef SOFTWARESERIAL_H
#define SOFTWARESERIAL_H

#include <Arduino.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size

class SoftwareSerial : public Stream {
  private:
    // per object data
    uint16_t _receivePin;
    uint16_t _transmitPin;
    GPIO_TypeDef *_receivePinPort;
    uint32_t _receivePinNumber;
    GPIO_TypeDef *_transmitPinPort;
    uint32_t _transmitPinNumber;
    uint32_t _speed;

    uint16_t _buffer_overflow: 1;
    uint16_t _inverse_logic: 1;
    uint16_t _half_duplex: 1;
    uint16_t _output_pending: 1;

    unsigned char _receive_buffer[_SS_MAX_RX_BUFF];
    volatile uint8_t _receive_buffer_tail;
    volatile uint8_t _receive_buffer_head;

    uint32_t delta_start = 0;

    // static data
    static bool initialised;
    static HardwareTimer timer;
    static SoftwareSerial *active_listener;
    static SoftwareSerial *volatile active_out;
    static SoftwareSerial *volatile active_in;
    static int32_t tx_tick_cnt;
    static volatile int32_t rx_tick_cnt;
    static uint32_t tx_buffer;
    static int32_t tx_bit_cnt;
    static uint32_t rx_buffer;
    static int32_t rx_bit_cnt;
    static uint32_t cur_speed;

    // private methods
    void send() NOEXCEPT;
    void recv() NOEXCEPT;
    void setTX() NOEXCEPT;
    void setRX() NOEXCEPT;
    void setSpeed(uint32_t speed) NOEXCEPT;
    void setRXTX(bool input) NOEXCEPT;
    static void handleInterrupt() NOEXCEPT;

  public:
    // public methods

    SoftwareSerial(uint16_t receivePin, uint16_t transmitPin, bool inverse_logic = false) NOEXCEPT;
    virtual ~SoftwareSerial() NOEXCEPT;
    void begin(long speed) NOEXCEPT;
    bool listen() NOEXCEPT;
    void end() NOEXCEPT;
    bool isListening() NOEXCEPT
    {
      return active_listener == this;
    }
    bool stopListening() NOEXCEPT;
    bool overflow() NOEXCEPT
    {
      bool ret = _buffer_overflow;
      if (ret) {
        _buffer_overflow = false;
      }
      return ret;
    }
    int peek() NOEXCEPT;

    virtual size_t write(uint8_t byte) NOEXCEPT;
    virtual int read() NOEXCEPT;
    virtual int available() NOEXCEPT;
    virtual void flush() NOEXCEPT;
    operator bool() NOEXCEPT
    {
      return true;
    }

    static void setInterruptPriority(uint32_t preemptPriority, uint32_t subPriority) NOEXCEPT;

    using Print::write;
};

#endif // SOFTWARESERIAL_H
