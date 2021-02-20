/*
 * AnalogInput.h
 *
 *  Created on: 2 Apr 2016
 *      Author: David
 */

#ifndef ANALOGIN_H_
#define ANALOGIN_H_
#include "pins_arduino.h"
#include "stm32f4xx_ll_adc.h"
#ifdef __cplusplus

// Module initialisation
void AnalogInInit() NOEXCEPT;

// Enable or disable a channel. Use AnalogCheckReady to make sure the ADC is ready before calling this.
void AnalogInEnableChannel(AnalogChannelNumber channel, bool enable) NOEXCEPT;

// Return the number of bits provided by a call to AnalogInReadChannel
//static constexpr unsigned int AdcBits = ADC_RESOLUTION;
// Hardware is 12bit we oversample up to 14
static constexpr unsigned int AdcBits = 14;
// ADC VREF and MCU Temperature calibration values
#define TEMPSENSOR_CAL1_DEF 931
#define TEMPSENSOR_CAL2_DEF 1197
#define VREFINT_CAL_DEF 1500
#define GET_ADC_CAL(CAL, DEF) (*CAL == 0xffff ? DEF : *CAL)
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
extern AnalogChannelNumber PinToAdcChannel(uint32_t pin) NOEXCEPT;

// Get the temperature measurement channel
extern AnalogChannelNumber GetTemperatureAdcChannel() NOEXCEPT;
extern AnalogChannelNumber GetVREFAdcChannel() NOEXCEPT;

#endif

#endif /* CORES_ARDUINO_ANALOGIN_H_ */
