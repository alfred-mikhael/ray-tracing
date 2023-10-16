/*
 * metal.h
 *
 *  Created on: Apr. 25, 2023
 *      Author: alfred
 */

#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include "../geometry/ray.h"
#include "../utils/color.h"
#include "material.h"

class metal : public material {
public:
	metal(const color& c, double f) : albedo(c), fuzz(f < 1 ? f : 1) {};

	virtual bool scatter(const ray& in, const hit_record& rec, color& atten, ray& scatter)
		const override {
		vec3 reflected = reflect(in.direction(), rec.normal);
		atten = albedo;
		scatter = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), in.time());
		return (dot(reflected, rec.normal) > 0);
	}

public:
	color albedo;
	double fuzz;
};


#endif /* MATERIALS_METAL_H_ */
