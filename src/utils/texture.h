/*
 * texture.h
 *
 *  Created on: Jun. 12, 2023
 *      Author: alfred
 */

#ifndef UTILS_TEXTURE_H_
#define UTILS_TEXTURE_H_

#include "color.h"
#include "utility.h"
#include "perlin.h"
#include "rtw_image.h"
#include "../geometry/vec3.h"
#include <cmath>

class texture {
public:
	virtual color value(double u, double v, const point3& p) const = 0;
	virtual ~texture() {};
};

class solid_color : public texture {
    public:
        solid_color() {}
        solid_color(color c) : color_value(c) {}

        solid_color(double red, double green, double blue)
          : solid_color(color(red,green,blue)) {}

        virtual inline color value(double u, double v, const vec3& p) const override {
            return color_value;
        }

    private:
        color color_value;
};

class checker_texture : public texture {
    public:
        checker_texture() : size(10) {}

        checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(_even), odd(_odd), size(10) {}

        checker_texture(color c1, color c2)
            : even(make_shared<solid_color>(c1)) , odd(make_shared<solid_color>(c2)), size(10) {}


        virtual color value(double u, double v, const point3& p) const override {
            auto sines = std::sin(static_cast<float>(size * p.x())) *
            		std::sin(static_cast<float>(size * p.y())) *
					std::sin(static_cast<float>(size * p.z()));
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

        void set_size(double new_size) {
        	size = new_size;
        }

    public:
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
        double size;
};

class image_texture : public texture {
  public:
    image_texture(const char* filename) : image(filename) {}

    color value(double u, double v, const point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return color(0,1,1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0, 1);
        v = 1.0 - clamp(v, 0, 1);  // Flip V to image coordinates

        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i,j);

        auto color_scale = 1.0 / 255.0;
        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

  private:
    rtw_image image;
};

class perlin_texture : public texture {
    public:
        perlin_texture() : noise(perlin(256)) {};
        perlin_texture(int n) : noise(perlin(n)) {};

        color value(double u, double v, const point3& p) const override {
            u = clamp(u, 0.0, 1.0);
            v = clamp(v, 0.0, 1.0);

            double value = noise.noise(u, v);
            // normalize value to be in [0, 1]
            value = 0.5 + 0.5 * value; 

            return color(value, value, value);
        }

    private: 
        perlin noise;
};

#endif /* UTILS_TEXTURE_H_ */
