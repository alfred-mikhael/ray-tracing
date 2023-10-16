/*
 * ray.h
 *
 *  Created on: Apr. 22, 2023
 *      Author: alfred
 */

#ifndef GEOMETRY_RAY_H_
#define GEOMETRY_RAY_H_

#include "vec3.h"

class ray {
public:
	ray() : tm(0) {}
	ray(const vec3& o, const vec3& d, double t) : orig(o), dir(d), tm(t) {}

	inline point3 origin() const {
		return orig;
	}

	inline vec3 direction() const {
		return dir;
	}

	double time() const {
		return tm;
	}

	point3 at(double t) const {
		return orig + t * dir;
	}

public:
	point3 orig;
	vec3 dir;
	double tm;
};



#endif /* GEOMETRY_RAY_H_ */
