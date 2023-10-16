/*
 * color.h
 *
 *  Created on: Apr. 21, 2023
 *      Author: alfred
 */

#ifndef UTILS_COLOR_H_
#define UTILS_COLOR_H_

#include "../geometry/vec3.h"
#include <iostream>
#include <cmath>
#include "utility.h"

using std::sqrt;

/**
 * Writes the RGB values of pixel to out. pixel should be a vector of color
 * components, so that 0 <= pixel.x(), pixel.y(), pixel.z() <= 1.
 *
 * @param out Output stream on which to write
 * @param pixel Vector of color components
 */
std::ostream& write_color(std::ostream& out, color pixel, int num_samples) {
	auto r = pixel.x();
	auto g = pixel.y();
	auto b = pixel.z();
	double scale = 1.0 / num_samples;

	// Gamma-2 adjustment
	r = sqrt(static_cast<float>(r * scale));
	g = sqrt(static_cast<float>(g * scale));
	b = sqrt(static_cast<float>(b * scale));

	return out << static_cast<int>(256 * clamp(r, 0, 0.999))
			<< ' ' << static_cast<int>(256 * clamp(g, 0, 0.999))
			<< ' ' << static_cast<int>(256 * clamp(b, 0, 0.999)) << "\n";
}



#endif /* UTILS_COLOR_H_ */
