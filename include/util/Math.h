/* Math.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Util functions and constants for math-related work
 */

#ifndef UTIL_MATH_H__
#define UTIL_MATH_H__

namespace Util {

constexpr double PI = 3.1415927;

inline double to_degrees(double radians) {
	return (radians * 180) / PI;
}

inline double to_radians(double angle) {
	return (angle * PI) / 180;
}

}	// Util

#endif
