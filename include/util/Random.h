/* Random.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Randomization-related utility functions.
 */

#ifndef UTIL_RANDOM_H__
#define UTIL_RANDOM_H__

namespace Util {

double rand_between(double min, double max);
float rand_between(float min, float max);
int rand_between(int min, int max);

}	// Util

#endif
