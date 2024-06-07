/*
 * material.h
 *
 *  Created on: Apr. 25, 2023
 *      Author: alfred
 */

#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_

#include "../utils/hittable.h"
#include "../utils/color.h"
#include "../geometry/ray.h"

struct hit_record;

class material {
public:
	virtual bool scatter(const ray& incoming, const hit_record& rec, color& atten, ray& scattered)
		const= 0;

	virtual color emitted(double u, double v, const point3& p) const {
		return color(0, 0, 0);
	}

	virtual ~material() {};
};



#endif /* MATERIALS_MATERIAL_H_ */
