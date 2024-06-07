#include "material.h"
#include "../utils/texture.h"
#include "../geometry/utils.h"

class diffuse_light : public material {

    public:
        diffuse_light(shared_ptr<texture> t) : tex(t) {}

        diffuse_light(const color& emit) : tex(std::make_shared<solid_color>(emit)) {}

        bool scatter(const ray& incoming, const hit_record& rec, color& atten, ray& scatter) const override {
            return false;
	    }
        
        color emitted(double u, double v, const point3& p) const override {
            return tex->value(u, v, p);
        }

    private:
        shared_ptr<texture> tex;
};