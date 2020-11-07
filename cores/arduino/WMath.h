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

#ifndef _WIRING_MATH_
#define _WIRING_MATH_

#ifdef __cplusplus
#include <algorithm>
using std::min;
using std::max;

long random32(long);
long random32(long, long);
void randomSeed(uint32_t dwSeed);
long map(long, long, long, long, long);

uint16_t makeWord(uint16_t w) ;
uint16_t makeWord(uint8_t h, uint8_t l) ;

#define word(...) makeWord(__VA_ARGS__)

#include <cmath>

// std::min and std::max don't seem to work with this variant of gcc, so define our own ones here
// We use these only with primitive types, so pass them directly instead of by const reference
#ifdef min
# undef min
#endif

#ifdef max
# undef max
#endif

// We need to use "extern C++" here so that it compiles even if this file was #included inside an "extern C" block
extern "C++" {

	static inline bool Xor(bool a, bool b) NOEXCEPT {
		return (a) ? !b : b;
	}

	static inline bool XNor(bool a, bool b) NOEXCEPT {
		return (a) ? b : !b;
	}

	static inline int32_t random(int32_t howbig) NOEXCEPT {
		return random32(static_cast<long>(howbig));
	}

	static inline int32_t random(int32_t howsmall, int32_t howbig) NOEXCEPT {
		return random32(static_cast<long>(howsmall), static_cast<long>(howbig));
	}

	// Note that constrain<float> will return NaN for a NaN input because of the way we define min<float> and max<float>
	template<typename T>
	inline constexpr const T& constrain(const T& val, const T& vmin, const T& vmax) NOEXCEPT {
		return std::clamp(val, vmin, vmax);
		//return (val < vmin) ? vmin : (val > vmax) ? vmax : val;
	}

	static inline constexpr float fsquare(float arg) NOEXCEPT {
		return arg * arg;
	}

	static inline constexpr double dsquare(double arg) NOEXCEPT {
		return arg * arg;
	}

	static inline constexpr uint64_t isquare64(int32_t arg) NOEXCEPT {
		return (uint64_t)((int64_t)arg * arg);
	}

	static inline constexpr uint64_t isquare64(uint32_t arg) NOEXCEPT {
		return (uint64_t)arg * arg;
	}

	// Find the lowest set bit. Returns the lowest set bit number, undefined if no bits are set.
	// GCC provides intrinsics, but unhelpfully they are in terms of int, long and long long instead of uint32_t, uint64_t etc.
	static inline unsigned int LowestSetBitNumber(unsigned int val) NOEXCEPT {
		return (unsigned int)__builtin_ctz(val);
	}

	static inline unsigned int LowestSetBitNumber(unsigned long val) NOEXCEPT {
		return (unsigned int)__builtin_ctzl(val);
	}

	static inline unsigned int LowestSetBitNumber(unsigned long long val) NOEXCEPT	{
		return (unsigned int)__builtin_ctzll(val);
	}
}

#endif

// Macro to give us the number of elements in an array
#define ARRAY_SIZE(_x)	(sizeof(_x)/sizeof(_x[0]))

// Macro to give us the highest valid index into an array i.e. one less than the size
#define ARRAY_UPB(_x)	(ARRAY_SIZE(_x) - 1)

#endif /* _WIRING_MATH_ */
