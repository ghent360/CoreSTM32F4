#ifndef HARDWARECRC32_H
#define HARDWARECRC32_H

#if defined(STM32F4xx)
#include "stm32f4xx_hal_crc.h"
#elif defined STM32F7xx
#include "stm32f7xx_hal_crc.h"
#else
#error "Architecture is not supported"
#endif


class HardwareCRC32
{
public:
    HardwareCRC32(CRC_TypeDef *Instance) noexcept;
    uint32_t CalcCRC32(const uint8_t val, const uint32_t crc) noexcept;
    uint32_t CalcCRC32(const uint8_t *vals, const size_t len, const uint32_t crc) noexcept;
private:
    uint32_t inline Calc(const uint8_t val, const uint32_t crc) noexcept __attribute__((always_inline));
    CRC_TypeDef *inst;
};

#endif