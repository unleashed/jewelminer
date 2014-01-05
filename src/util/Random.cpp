/* Random.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Randomization-related utility functions.
 */

#include <random>
#include <chrono>

#include <type_traits>
#include <cmath>
#include <limits>

#include "util/Random.h"

namespace Util {

// statically seed the RNG
static std::default_random_engine RNgenerator(std::chrono::system_clock::now().time_since_epoch().count());

double rand_between(double min, double max)
{
	std::uniform_real_distribution<double> distribution(min, std::nextafter(max, std::numeric_limits<double>::max()));
	return distribution(RNgenerator);
}

float rand_between(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, std::nextafter(max, std::numeric_limits<float>::max()));
	return distribution(RNgenerator);
}

int rand_between(int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(RNgenerator);
}

}	// Util
