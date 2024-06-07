/*
 * lambertian.h
 *
 *  Created on: Apr. 25, 2023
 *      Author: alfred
 */

#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "material.h"
#include "../geometry/utils.h"
#include "../utils/texture.h"

class lambertian : public material {
public:
	lambertian(const color& c) : albedo(make_shared<solid_color>(c)) {};
    lambertian(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(const ray& incoming, const hit_record& rec, color& atten, ray& scatter)
		const override {
		auto scatter_direction = random_in_hemisphere(rec.normal);

		// This code will never be reached because random_in_hemisphere will never 
		// return a value opposite to the normal.
		// if (scatter_direction.near_zero()) {
		// 	scatter_direction = rec.normal;
		// }

		scatter = ray(rec.p, scatter_direction, incoming.time());
		atten = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	std::shared_ptr<texture> albedo;
};




#endif /* MATERIALS_LAMBERTIAN_H_ */
