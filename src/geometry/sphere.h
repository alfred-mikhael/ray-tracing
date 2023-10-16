/*
 * sphere.h
 *
 *  Created on: Apr. 24, 2023
 *      Author: alfred
 */

#ifndef GEOMETRY_SPHERE_H_
#define GEOMETRY_SPHERE_H_

#include "../utils/hittable.h"
#include "vec3.h"
#include "ray.h"
#include "utils.h"
#include "../utils/utility.h"
#include <cmath>

class sphere : public hittable {
public:
	sphere(const point3& cen0, const point3& cen1, double t0, double t1, double radius, shared_ptr<material> m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(radius), mat(m) {};

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;

	point3 center(const double t) const {
		auto s = clamp(t, time0, time1);
		return center0 + ((s - time0) / (time1 - time0)) * (center1 - center0);
	}

private:
    static void get_sphere_uv(const point3& p, double& u, double& v) {
    	// u, v are the normalized spherical coordinates (phi, theta), where
    	// u is the azimuthal angle and v is the polar angle.
        auto theta = std::acos(static_cast<float>(-p.y()));
        auto phi = std::atan2(static_cast<float>(-p.z()), static_cast<float>(p.x())) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }

public:
	point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center(r.time());
	double a = dot(r.direction(), r.direction());
	double b = dot(r.direction(), oc);
	double c = dot(oc, oc) - radius * radius;
	float disc = b * b - a * c;

	if (disc < 0) {
		return false;
	}

	auto sqrtd = sqrt(disc);
	double root = (-b - sqrtd) / a;
	if (root < tmin || root > tmax) {
		root = (-b + sqrtd) / a;
		if (root < tmin || root > tmax) {
			return false;
		}
	}

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center(r.time())) / radius;
    get_sphere_uv(rec.normal, rec.u, rec.v);
    rec.mat_ptr = mat;

    return true;
}

bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
	vec3 radius_vec = vec3(radius, radius, radius);
	aabb box0 = aabb(center(t0) - radius_vec, center(t0) + radius_vec);
	aabb box1 = aabb(center(t1) - radius_vec, center(t1) + radius_vec);
	output_box = surrounding_box(box0, box1);
	return true;
}

inline std::ostream& operator<<(std::ostream& out, const shared_ptr<sphere>& v) {
	return out << v->center0 << " : " << v->radius;
}

#endif /* GEOMETRY_SPHERE_H_ */
