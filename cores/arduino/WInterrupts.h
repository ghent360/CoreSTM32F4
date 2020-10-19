/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#ifndef _WIRING_INTERRUPTS_
#define _WIRING_INTERRUPTS_

#include <stdint.h>
#include "wiring_constants.h"

#include <functional>

typedef std::function<void(void) NOEXCEPT> callback_function_t;
void attachInterrupt(uint32_t pin, callback_function_t callback, enum InterruptMode mode) NOEXCEPT;

struct CallbackParameter
{
  callback_function_t fp;
  void *vp;
  uint32_t u32;
  int32_t i32;

  CallbackParameter(callback_function_t pp) : fp(pp) {}
  CallbackParameter(void *pp) NOEXCEPT : vp(pp) { }
  CallbackParameter(uint32_t pp) NOEXCEPT : u32(pp) { }
  CallbackParameter(int32_t pp) NOEXCEPT : i32(pp) { }
  CallbackParameter() NOEXCEPT : u32(0) { }
};

void StandardCallbackFunctionPrototype(CallbackParameter) NOEXCEPT;
using StandardCallbackFunction = decltype(&StandardCallbackFunctionPrototype);

bool attachInterrupt(uint32_t pin, StandardCallbackFunction callback, enum InterruptMode mode, CallbackParameter param) NOEXCEPT;

extern "C" {

void attachInterrupt(uint32_t pin, void (*callback)(void), enum InterruptMode mode);
void detachInterrupt(uint32_t pin);
// Return true if we are in an interrupt service routine
//bool inInterrupt();

} // extern "C"

#endif /* _WIRING_INTERRUPTS_ */
