/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

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

#ifndef Print_h
#define Print_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include <stdarg.h> // for printf

#include "WString.h"
#include "Printable.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Print {
  private:
    int write_error;
    size_t printNumber(unsigned long, uint8_t) NOEXCEPT;
    size_t printULLNumber(unsigned long long, uint8_t) NOEXCEPT;
    size_t printFloat(double, uint8_t) NOEXCEPT;
  protected:
    void setWriteError(int err = 1) NOEXCEPT
    {
      write_error = err;
    }
  public:
    Print() : write_error(0) {}

    int getWriteError() NOEXCEPT
    {
      return write_error;
    }
    void clearWriteError() NOEXCEPT
    {
      setWriteError(0);
    }

    virtual size_t write(uint8_t) NOEXCEPT = 0;
    size_t write(const char *str) NOEXCEPT
    {
      if (str == NULL) {
        return 0;
      }
      return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size) NOEXCEPT;
    size_t write(const char *buffer, size_t size) NOEXCEPT
    {
      return write((const uint8_t *)buffer, size);
    }

    // default to zero, meaning "a single write may block"
    // should be overriden by subclasses with buffering
    virtual int availableForWrite() NOEXCEPT
    {
      return 0;
    }

    size_t print(const __FlashStringHelper *) NOEXCEPT;
    size_t print(const String &) NOEXCEPT;
    size_t print(const char[]) NOEXCEPT;
    size_t print(char) NOEXCEPT;
    size_t print(unsigned char, int = DEC) NOEXCEPT;
    size_t print(int, int = DEC) NOEXCEPT;
    size_t print(unsigned int, int = DEC) NOEXCEPT;
    size_t print(long, int = DEC) NOEXCEPT;
    size_t print(unsigned long, int = DEC) NOEXCEPT;
    size_t print(long long, int = DEC) NOEXCEPT;
    size_t print(unsigned long long, int = DEC) NOEXCEPT;
    size_t print(double, int = 2) NOEXCEPT;
    size_t print(const Printable &) NOEXCEPT;

    size_t println(const __FlashStringHelper *) NOEXCEPT;
    size_t println(const String &s) NOEXCEPT;
    size_t println(const char[]) NOEXCEPT;
    size_t println(char) NOEXCEPT;
    size_t println(unsigned char, int = DEC) NOEXCEPT;
    size_t println(int, int = DEC) NOEXCEPT;
    size_t println(unsigned int, int = DEC) NOEXCEPT;
    size_t println(long, int = DEC) NOEXCEPT;
    size_t println(unsigned long, int = DEC) NOEXCEPT;
    size_t println(long long, int = DEC) NOEXCEPT;
    size_t println(unsigned long long, int = DEC) NOEXCEPT;
    size_t println(double, int = 2) NOEXCEPT;
    size_t println(const Printable &) NOEXCEPT;
    size_t println(void) NOEXCEPT;

    int printf(const char *format, ...) NOEXCEPT;
    int printf(const __FlashStringHelper *format, ...) NOEXCEPT;

    virtual void flush() NOEXCEPT { /* Empty implementation for backward compatibility */ }
};

#endif
