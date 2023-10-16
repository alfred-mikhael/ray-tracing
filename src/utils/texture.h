/*
 * texture.h
 *
 *  Created on: Jun. 12, 2023
 *      Author: alfred
 */

#ifndef UTILS_TEXTURE_H_
#define UTILS_TEXTURE_H_

#include "color.h"
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

#endif /* UTILS_TEXTURE_H_ */
