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

// hittable_list random_scene() {
//     hittable_list world;
//
//     auto earth = make_shared<image_texture>("earthmap.jpg");
//     auto earth_text = make_shared<lambertian>(earth);
//     world.add(make_shared<sphere>(point3(0,-1000,0), point3(0,-1000,0), 0, 1, 1000, earth_text));
//
//     for (int a = -15; a < 15; a++) {
//         for (int b = -15; b < 15; b++) {
//             auto choose_mat = random_double();
//             point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
//
//             if ((center - point3(4, 0.2, 0)).length() > 0.9) {
//                 shared_ptr<material> sphere_material;
//
//                 if (choose_mat < 0.8) {
//                     // diffuse
//                     auto albedo = color::random() * color::random();
//                     auto jump = vec3(0, random_double(0, 0.5), 0);
//                     sphere_material = make_shared<lambertian>(albedo);
//                     world.add(make_shared<sphere>(center, center + jump, 0, 1, 0.2, sphere_material));
//                 } else if (choose_mat < 0.95) {
//                     // metal
//                     auto albedo = color::random(0.5, 1);
//                     auto fuzz = random_double(0, 0.5);
//                     sphere_material = make_shared<metal>(albedo, fuzz);
//                     world.add(make_shared<sphere>(center, center, 0, 1, 0.2, sphere_material));
//                 } else {
//                     // glass
//                     sphere_material = make_shared<dielectric>(1.5);
//                     world.add(make_shared<sphere>(center, center, 0, 1, 0.2, sphere_material));
//                 }
//             }
//         }
//     }
//     auto material1 = make_shared<dielectric>(1.5);
//     world.add(make_shared<sphere>(point3(0, 1, 0), point3(0, 1, 0), 0, 1, 1.0, material1));
//
//     auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//     world.add(make_shared<sphere>(point3(-4, 1, 0), point3(-4, 1, 0), 0, 1, 1.0, material2));
//
//     auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//     world.add(make_shared<sphere>(point3(4, 1, 0), point3(4, 1, 0), 0, 1, 1.0, material3));
//
//     return world;
// }

// hittable_list earth_scene() {
//     hittable_list world;
//
//     auto earth = make_shared<image_texture>("earthmap.jpg");
//     auto earth_text = make_shared<lambertian>(earth);
//     world.add(make_shared<sphere>(point3(0,0,0), point3(0,0,0), 0, 1, 3.0, earth_text));
//
//     return world;
// }

// hittable_list perlin_scene() {
//     hittable_list world;
//
//     auto perlin = make_shared<perlin_texture>();
//     auto perlin_text = make_shared<lambertian>(perlin);
//     world.add(make_shared<sphere>(point3(0,0,0), point3(0,0,0), 0, 1, 3.0, perlin_text));
//
//     return world;
// }

hittable_list quads() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam(80, point3(0, 0, 9), point3(0, 0, 0), vec3(0, 1, 0), 1.0, 1.0, 0.1, 5.0, 0.0, 1.0);
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    bvh_node bvh_world(world);

    std::ofstream output;
    output.open("image2.ppm");

    cam.render(output, bvh_world);
}

void cornell_box() {
    // copied from Ray Tracing: The Next Week

    // set camera params
    double aspect_ratio = 1.0;
    int image_width = 600;
    int image_height = 600;
    double vfov = 40;
    point3 lookfrom = point3(278, 278, -800);
    point3 lookat = point3(278, 278, 0);
    point3 vup = vec3(0,1,0);
    
    camera cam(vfov, lookfrom, lookat, vup, 1.0, 1.0, 0.1, 800.0, 0.0, 1.0);
    cam.samples_per_pixel = 200;
    cam.max_depth = 100;
    cam.image_height = image_height;
    cam.image_width = image_width;
    
    // construct world
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    bvh_node bvh_world(world, 0.0, 1.0);

    // render
    std::ofstream output;
    output.open("image2.ppm");

    cam.render(output, bvh_world);
}

int main() {
    cornell_box();
	return 0;
}



