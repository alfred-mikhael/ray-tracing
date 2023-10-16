/*
 * hittable.h
 *
 *  Created on: Apr. 24, 2023
 *      Author: alfred
 */

#ifndef UTILS_HITTABLE_H_
#define UTILS_HITTABLE_H_

#include "../geometry/ray.h"
#include "../geometry/aabb.h"
#include "../materials/material.h"

#include <memory>
using std::shared_ptr;

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v;
};

class hittable {
public:
	virtual ~hittable() {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const = 0;
	virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
};


#endif /* UTILS_HITTABLE_H_ */
