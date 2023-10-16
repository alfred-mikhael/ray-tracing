/*
 * camera.h
 *
 *  Created on: Apr. 24, 2023
 *      Author: alfred
 */

#ifndef UTILS_CAMERA_H_
#define UTILS_CAMERA_H_

#include "../geometry/ray.h"
#include "utility.h"


class camera {
public:
	camera(
			double vfov,
			point3 lookfrom,
			point3 lookat,
			vec3 vup,
			double ap,
			double fd,
			double time0,
			double time1
		) : camera(vfov, lookfrom, lookat, vup, 16.0 / 9.0, 1.0, ap, fd, time0, time1) {};

	camera(
			double vfov,
			point3 lookfrom,
			point3 lookat,
			vec3 vup,
			double ar,
			double fl,
			double aperture,
			double focus_dist,
			double time0,
			double time1
		) : time0(time0), time1(time1) {
		auto theta = degrees_to_radians(vfov);
        auto h = std::tan(static_cast<float>(theta / 2.0));
        double viewport_height = 2.0 * h;
        double viewport_width = ar * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

        lens_radius = aperture / 2;
	}

	ray get_ray(double h_offset, double v_offset) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(
            origin + offset,
            lower_left_corner + h_offset * horizontal + v_offset * vertical - origin - offset,
			random_double(time0, time1)
        );
	}

private:
	double lens_radius;
	vec3 u, v, w;
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	double time0, time1;
};



#endif /* UTILS_CAMERA_H_ */
