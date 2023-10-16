/*
 * aabb.h
 *
 *  Created on: Apr. 26, 2023
 *      Author: alfred
 */

#ifndef GEOMETRY_AABB_H_
#define GEOMETRY_AABB_H_

#include "ray.h"
#include <utility>
#include <algorithm>
// TODO: Get std::swap to work

class aabb {
public:
	aabb() {}
	aabb(point3 min, point3 max) : minimum(min), maximum(max) {}

	inline point3 min() const {
		return minimum;
	}
	inline point3 max() const {
		return maximum;
	}

	/**
	 * Optimized AABB intersection code taken from "Ray Tracing: The next week".
	 */
	inline bool hit(const ray& r, double t_min, double t_max) const {
	    const auto& origin = r.origin();
	    const auto& direction = r.direction();
	    const auto& minVal = min();
	    const auto& maxVal = max();

	    for (int a = 0; a < 3; a++) {
	        auto invD = 1.0f / direction[a];
	        double t0 = (minVal[a] - origin[a]) * invD;
	        double t1 = (maxVal[a] - origin[a]) * invD;
	        if (invD < 0.0f) {
	        	std::swap(t0, t1);
	        }
	        t_min = t0 > t_min ? t0 : t_min;
	        t_max = t1 < t_max ? t1 : t_max;
	        if (t_max <= t_min)
	            return false;
	    }
	    return true;
	}

	inline double surface_area() const {
		auto dx = maximum[0] - minimum[0];
		auto dy = maximum[1] - minimum[1];
		auto dz = maximum[2] - minimum[2];
		return 2 * (dx * dy + dx * dz + dy * dz);
	}

public:
	point3 minimum;
	point3 maximum;
};

aabb surrounding_box(const aabb& box0, const aabb& box1) {
    point3 min(std::fmin(box0.min().x(), box1.min().x()),
                 std::fmin(box0.min().y(), box1.min().y()),
                 std::fmin(box0.min().z(), box1.min().z()));

    point3 max(std::fmax(box0.max().x(), box1.max().x()),
               std::fmax(box0.max().y(), box1.max().y()),
               std::fmax(box0.max().z(), box1.max().z()));

    return aabb(min, max);
}

#endif /* GEOMETRY_AABB_H_ */
