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
	camera() {};

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

	color ray_color(const ray& r, const hittable& world, int depth) {
		if (depth <= 0) {
			return color(0, 0, 0);
		}

		hit_record rec;
		if (world.hit(r, 0.001, infinity, rec)) {
			ray scattered;
			color atten;
			color from_emisson = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

			if (rec.mat_ptr->scatter(r, rec, atten, scattered)) {
				return from_emisson + atten * ray_color(scattered, world, depth-1);
			} 
			// If no scattering, just return emitted light
			return from_emisson;
		}
		// no hit, return background color
		return color(0.0, 0.0, 0.0);
	}

	void render(std::ostream& output, const hittable& world, bool debug=true) {
		// Render
		output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int i = image_height - 1; i >= 0; i--) {
			
			if (debug) {
				std::cout << "writing row " << i << std::endl;
			}

			for (int j = 0; j < image_width; j++) {
				color pixel_color(0, 0, 0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					double h_offset = (j + random_double()) / (image_width-1);
					double v_offset = (i + random_double()) / (image_height-1);
					ray r = get_ray(h_offset, v_offset);
					pixel_color += ray_color(r, world, max_depth);
				}
				write_color(output, pixel_color, samples_per_pixel);
			}
		}
	}

	double lens_radius;
	vec3 u, v, w;
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	double time0, time1;
	int image_width=400;
	int image_height=225;
	int samples_per_pixel=50;
	int max_depth=30;
};



#endif /* UTILS_CAMERA_H_ */
