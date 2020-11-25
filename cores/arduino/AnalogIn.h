/*
 * AnalogInput.h
 *
 *  Created on: 2 Apr 2016
 *      Author: David
 */

#ifndef ANALOGIN_H_
#define ANALOGIN_H_
#include "pins_arduino.h"
#ifdef __cplusplus

// Module initialisation
void AnalogInInit() NOEXCEPT;

// Enable or disable a channel. Use AnalogCheckReady to make sure the ADC is ready before calling this.
void AnalogInEnableChannel(AnalogChannelNumber channel, bool enable) NOEXCEPT;

// Return the number of bits provided by a call to AnalogInReadChannel
static constexpr unsigned int AdcBits = ADC_RESOLUTION;

// Read the most recent result from a channel
uint16_t AnalogInReadChannel(AnalogChannelNumber channel) NOEXCEPT;

void AbalogCallbackFunction(void) NOEXCEPT;
using AnalogCallback_t = decltype(&AbalogCallbackFunction);
//typedef void (*AnalogCallback_t)(void) NOEXCEPT;

// Set up a callback for when all conversions have been completed. Returns the previous callback pointer.
AnalogCallback_t AnalogInSetCallback(AnalogCallback_t) NOEXCEPT;

// Start converting the enabled channels, to include the specified ones. Disabled channels are ignored.
void AnalogInStartConversion(uint32_t channels = 0xFFFFFFFF) NOEXCEPT;


void AnalogInFinaliseConversion() NOEXCEPT;

// Check whether all conversions of the specified channels have been completed since the last call to AnalogStartConversion.
// Disabled channels are ignored
bool AnalogInCheckReady(uint32_t channels = 0xFFFFFFFF) NOEXCEPT;

// Convert a pin number to an AnalogIn channel
extern AnalogChannelNumber PinToAdcChannel(PinName pin) NOEXCEPT;

// Get the temperature measurement channel
extern AnalogChannelNumber GetTemperatureAdcChannel() NOEXCEPT;

#endif

#endif /* CORES_ARDUINO_ANALOGIN_H_ */
