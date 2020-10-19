/*
 * Povide PWM based output using either Hardware or Software PWM.
 * GA 14/8/2020
 */
#ifndef SRC_HYBRIDPWM_H_
#define SRC_HYBRIDPWM_H_
#include "Core.h"
#include "General/StringRef.h"

constexpr uint32_t MaxPWMChannels = 16;

class HybridPWMPin;
class HybridPWMBase
{
    public:
        HybridPWMBase() NOEXCEPT;
        virtual void free() NOEXCEPT = 0;
        virtual void setValue(float val) NOEXCEPT = 0;
        virtual void appendStatus(const StringRef& reply) NOEXCEPT = 0;
        static HybridPWMBase *allocate(HybridPWMPin *p, Pin pin, uint32_t freq, float value) NOEXCEPT;

    protected:
        HybridPWMPin *pwmPin;
};

class HybridPWMPin
{
    public:
        HybridPWMPin() NOEXCEPT;
        static HybridPWMPin *allocate(Pin pin, float value) NOEXCEPT;
        static HybridPWMPin *find(Pin pin) NOEXCEPT;
        void free() NOEXCEPT;
        void set(float value, uint32_t freq) NOEXCEPT;
        void appendStatus(const StringRef& reply) NOEXCEPT;

        Pin pin;
        float value;
        uint32_t freq;

    private:
        HybridPWMBase *pwm;
};

class HardwarePWM : HybridPWMBase
{
    public:
        HardwarePWM() NOEXCEPT;
        virtual void free() NOEXCEPT;
        virtual void setValue(float val) NOEXCEPT;
        virtual void appendStatus(const StringRef& reply) NOEXCEPT;
        static HybridPWMBase *allocate(Pin pin, uint32_t freq, float value) NOEXCEPT;
    private:
        HardwareTimer *timer;
        uint32_t channel;
};

class SoftwarePWM : HybridPWMBase
{
    public:
        SoftwarePWM() NOEXCEPT;
        virtual void free() NOEXCEPT;
        virtual void setValue(float val) NOEXCEPT;
        virtual void appendStatus(const StringRef& reply) NOEXCEPT;
        static HybridPWMBase *allocate(Pin pin, uint32_t freq, float value) NOEXCEPT;
    private:
        void setValue(Pin pin, float value) NOEXCEPT;
        int32_t channel;
        uint32_t period;
};

extern HybridPWMPin PWMPins[MaxPWMChannels];
#endif