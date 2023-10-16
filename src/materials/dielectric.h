/*
 * dielectric.h
 *
 *  Created on: Apr. 25, 2023
 *      Author: alfred
 */

#ifndef MATERIALS_DIELECTRIC_H_
#define MATERIALS_DIELECTRIC_H_

#include "material.h"

class dielectric : public material {
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {};

	virtual bool scatter(const ray& in, const hit_record& rec, color& atten, ray& scatter)
		const override {
		atten = color(1.0, 1.0, 1.0);
		vec3 uv = unit_vector(in.direction());
		bool outwards_face = dot(uv, rec.normal) < 0;
		vec3 face_normal = outwards_face ? rec.normal : -rec.normal;

		// Snell's law is eta * sin theta = eta' * sin theta'
		// The normal is always facing outwards, so when a ray comes from outside,
		// solve for sin theta' so use eta / eta' = 1/eta' in air.
		double refraction_ratio = outwards_face ? (1.0 / ir) : ir;

		double cos_theta = std::fmin(-dot(uv, face_normal), 1.0);
		double sin_theta = std::sqrt(static_cast<float>(1.0 - cos_theta * cos_theta));

		bool can_refract = refraction_ratio * sin_theta <= 1;
		vec3 scatter_direction = reflect(uv, face_normal);
		if (can_refract && reflectance(cos_theta, refraction_ratio) < random_double()) {
			scatter_direction = refract(uv, face_normal, refraction_ratio);
		}

		scatter = ray(rec.p, scatter_direction, in.time());
		return true;
	}

public:
	double ir;

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};


#endif /* MATERIALS_DIELECTRIC_H_ */
