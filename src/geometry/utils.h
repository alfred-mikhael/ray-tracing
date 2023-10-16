/*
 * utils.h
 *
 *  Created on: Apr. 21, 2023
 *      Author: alfred
 */

#ifndef GEOMETRY_UTILS_H_
#define GEOMETRY_UTILS_H_

#include "vec3.h"

/**
 * True Lambertian diffusion.
 */
inline vec3 random_on_unit_sphere() {
	while (true) {
		vec3 v = vec3::random();
		if (v.length_squared() <= 1) {
			return unit_vector(v);
		}
	}
}

/**
 * False Lambertian diffusion
 * More rays are being reflected in the direction of the normal.
 */
inline vec3 random_in_unit_sphere() {
	while (true) {
		vec3 v = vec3::random();
		if (v.length_squared() <= 1) {
			return v;
		}
	}
}

/**
 * Uniform diffusion
 * Rays are reflected uniformly across all angles.
 */
inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 u = random_in_unit_sphere();
	if (dot(u, normal) > 0.0) {
		return u;
	} return -u;
}

#endif /* GEOMETRY_UTILS_H_ */
