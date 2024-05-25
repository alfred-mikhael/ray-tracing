//============================================================================
// Name        : ray_tracing.cpp
// Author      : Alfred Mikhael
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>

#include "geometry/geometry.h"
#include "utils/utils.h"
#include "materials/materials.h"

using std::make_shared;

color ray_color(const ray& r, const hittable& world, int depth) {
	if (depth <= 0) {
		return color(0, 0, 0);
	}

	hit_record rec;
	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color atten;
		if (rec.mat_ptr->scatter(r, rec, atten, scattered)) {
			return atten * ray_color(scattered, world, depth-1);
		} return color(0, 0, 0);
	}
	auto unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    checker->set_size(10);
    auto checkered_text = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(point3(0,-1000,0), point3(0,-1000,0), 0, 1, 1000, checkered_text));

    for (int a = -15; a < 15; a++) {
        for (int b = -15; b < 15; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    auto jump = vec3(0, random_double(0, 0.5), 0);
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, center + jump, 0, 1, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, center, 0, 1, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, center, 0, 1, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), point3(0, 1, 0), 0, 1, 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), point3(-4, 1, 0), 0, 1, 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), point3(4, 1, 0), 0, 1, 1.0, material3));

    return world;
}

int main() {
	// Image
	const int image_width = 400;
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 30;
	const int max_depth = 15;

	// World
	hittable_list world = random_scene();
	auto bvh_world = bvh_node(world, 0, 1);

	// Camera
    camera cam(20.0, point3(13,2,3), point3(0,0,0), vec3(0,1,0), 0.1, 10.0, 0, 1);

    std::ofstream output;
    output.open("image2.ppm");

	// Render
	output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i = image_height - 1; i >= 0; i--) {
		for (int j = 0; j < image_width; j++) {
            std::cout << "writing pixel (" << i << "," << j << ")" << std::endl;
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double h_offset = (j + random_double()) / (image_width-1);
                double v_offset = (i + random_double()) / (image_height-1);
                ray r = cam.get_ray(h_offset, v_offset);
                pixel_color += ray_color(r, bvh_world, max_depth);
            }
            write_color(output, pixel_color, samples_per_pixel);
		}
	}

	return 0;
}



