/*
 * Povide PWM based output using either Hardware or Software PWM.
 * GA 14/8/2020
 */

#include "HybridPWM.h"
extern "C" void debugPrintf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
HybridPWMPin PWMPins[MaxPWMChannels];

HybridPWMPin::HybridPWMPin() NOEXCEPT : pin(NoPin), value(-1.0f), freq(0), pwm(nullptr)
{

}

HybridPWMPin* HybridPWMPin::find(uint32_t ulPin) NOEXCEPT
{
    for(uint32_t i = 0; i < MaxPWMChannels; i++)
        if (PWMPins[i].pin == ulPin)
            return &PWMPins[i];
    return nullptr;
}


HybridPWMPin* HybridPWMPin::allocate(uint32_t ulPin, float value) NOEXCEPT
{
    // find a free channel and allocate it

    if (find(ulPin))
    {
        debugPrintf("Pin is already allocated as pwm %lx\n", ulPin);
        return nullptr;
    }
    HybridPWMPin *ret = find(NoPin);
    if (ret == nullptr)
    {
        debugPrintf("No PWM channels available for %lx\n", ulPin);
        return nullptr;
    }
    ret->pin = ulPin;
    ret->value = value;
    ret->pwm = nullptr;
    ret->freq = 0;
    // set initial state
    pinMode(ulPin, (value < 0.5) ? OUTPUT_LOW : OUTPUT_HIGH);
    // grab the hardware if we can
    //ret->pwm = HybridPWMBase::allocate(ret, pin, 0, value);
    return ret;
}

void HybridPWMPin::free() NOEXCEPT
{
    pinMode(pin, OUTPUT_LOW);
    if (pwm) pwm->free();
    pin = NoPin;
    freq = 0;
    value = -1.0f;
    pwm = nullptr;
}

void HybridPWMPin::set(float value, uint32_t freq) NOEXCEPT
{
    if (this->freq != freq)
    {
        if (pwm) pwm->free();
        if (freq != 0)
        {
            pwm = HybridPWMBase::allocate(this, pin, freq, value);
        }
        this->freq = freq;
        this->value = value;
    }
    if (this->value != value)
    {
        if (pwm) 
            pwm->setValue(value);
        else
            pinMode(pin, (value < 0.5) ? OUTPUT_LOW : OUTPUT_HIGH);
        this->value = value;
    }
}

void HybridPWMPin::appendStatus(const StringRef& reply) NOEXCEPT
{
    if (pin == (uint32_t)NC) return;
    reply.catf(" Pin %c.%d freq %d value %f", 
        (uint8_t)((pin >> 4) + 'A'), (uint8_t)pin & 0xf, static_cast<int>(freq), (double)value);
    if (pwm) pwm->appendStatus(reply);
}

HybridPWMBase::HybridPWMBase() NOEXCEPT : pwmPin(nullptr)
{
}


HybridPWMBase *HybridPWMBase::allocate(HybridPWMPin *p, uint32_t ulPin, uint32_t freq, float value) NOEXCEPT
{
    HybridPWMBase *ret = HardwarePWM::allocate(ulPin, freq, value);
    if (!ret)
        ret = SoftwarePWM::allocate(ulPin, freq, value);
    if (ret)
        ret->pwmPin = p;
    return ret;
}
