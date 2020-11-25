/*
 Copyright (c) 2011 Arduino.  All right reserved.

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

#ifdef SUPPORT_ANALOG_OUT
#include "Core.h"
#include "AnalogOut.h"
#include "HybridPWM.h"
extern "C" void debugPrintf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
// Analog write to DAC, PWM, TC or plain output pin
// Setting the frequency of a TC or PWM pin to zero resets it so that the next call to AnalogOut with a non-zero frequency
// will re-initialise it. The pinMode function relies on this.
void AnalogOut(uint32_t ulPin, float ulValue, PwmFrequency freq)
{
	if (ulPin == (uint32_t)NC) return;
	ulValue = constrain<float>(ulValue, 0.0, 1.0);
	HybridPWMPin* hp = HybridPWMPin::find(ulPin);
	if (hp == nullptr)
	{
		debugPrintf("Trying set set analog value for unallocated pin %lx\n", ulPin);
		return;
	}
	hp->set(ulValue, freq);
}

void ReleasePWMPin(uint32_t ulPin)
{
	HybridPWMPin* hp = HybridPWMPin::find(ulPin);
	if (hp == nullptr)
	{
		debugPrintf("Release of unallocated PWM pin %lx\n", ulPin);
		return;
	}
	debugPrintf("Release of allocated PWM pin %lx\n", ulPin);
	hp->free();
}
// End
#endif // SUPPORT_ANALOG_OUT
