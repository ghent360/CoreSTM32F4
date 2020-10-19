/*
 * Cache.h
 *
 *  Created on: 22 Nov 2019
 *      Author: David
 */

#ifndef SRC_HARDWARE_CACHE_H_
#define SRC_HARDWARE_CACHE_H_

#include <Core.h>

namespace Cache
{
  void Init() NOEXCEPT;
  void Enable() NOEXCEPT;
  void Disable() NOEXCEPT;
  void Flush(const volatile void *start, size_t length) NOEXCEPT;
  void Invalidate(const volatile void *start, size_t length) NOEXCEPT;

  inline void FlushBeforeDMAReceive(const volatile void *start, size_t length) NOEXCEPT { Flush(start, length); }
  inline void InvalidateAfterDMAReceive(const volatile void *start, size_t length) NOEXCEPT { Invalidate(start, length); }
  inline void FlushBeforeDMASend(const volatile void *start, size_t length) NOEXCEPT { Flush(start, length); }
};

// Entry points that can be called from ASF C code
extern "C" void CacheFlushBeforeDMAReceive(const volatile void *start, size_t length) NOEXCEPT;
extern "C" void CacheInvalidateAfterDMAReceive(const volatile void *start, size_t length) NOEXCEPT;
extern "C" void CacheFlushBeforeDMASend(const volatile void *start, size_t length) NOEXCEPT;

// LPC178x processors have no cache
inline void Cache::Init() NOEXCEPT { __DSB(); }
inline void Cache::Enable() NOEXCEPT { }
inline void Cache::Disable() NOEXCEPT { }
inline void Cache::Flush(const volatile void *start, size_t length) NOEXCEPT { __DSB(); }
inline void Cache::Invalidate(const volatile void *start, size_t length) NOEXCEPT { __DSB(); }


#endif /* SRC_HARDWARE_CACHE_H_ */
