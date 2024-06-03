#ifndef quad
#define QUAD_H

#include "../utils/hittable.h"
#include "vec3.h"
#include "..//materials/material.h"
#include "aabb.h"

class quad : public hittable {
    public:
        quad(const point3& Q, const vec3& v, const vec3& u, shared_ptr<material> mat) :
        Q(Q), v(v), u(u), mat(mat) {
            set_bbox();
            normal = unit_vector(cross(u, v));
            D = dot(normal, Q);
            w = cross(u, v) / cross(u, v).length_squared();
        }

        bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
            double denom = dot(normal, r.direction());
            
            if (std::fabs(denom) < 0.0001) {
                // parallel, no hit
                return false;
            }
            double t = (D - dot(normal, r.orig)) / denom;
            point3 intersection = r.at(t);

            std::pair<double, double> coords = get_uv_basis_coordinates(intersection);
            
            if (tmin <= t && t <= tmax && point_lies_interior(coords.first, coords.second)) {
                rec.t = t;
                rec.mat_ptr = mat;
                rec.p = intersection;

                bool outwards = dot(normal, r.dir) < 0;
                rec.normal = outwards ? normal : -normal;

                rec.u = coords.first;
                rec.v = coords.second;
                return true;
            } 
            return false;
        }
        
        bool bounding_box(double t0, double t1, aabb& output_box) const {
            output_box = bbox;
            return true;
        }

        std::pair<double, double> get_uv_basis_coordinates(const point3& p) const {
            point3 centered_p = p - Q;
            double alpha = dot(w, cross(centered_p, v));
            double beta = dot(w, cross(u, centered_p));
            return std::pair<double, double>(alpha, beta);
        }

        bool point_lies_interior(double u_coord, double v_coord) const {
            return (0 <= u_coord && u_coord <= 1 && 0 <= v_coord && v_coord <= 1);
        }

    private:
        point3 Q;
        vec3 v, u;
        shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;
        vec3 w;
        double D;

        void set_bbox() {
            bbox = aabb(Q, Q + u + v);
            // auto bbox1 = aabb(Q, Q + u + v);
            // auto bbox2 = aabb(Q + u, Q + v);
            // bbox = surrounding_box(bbox1, bbox2);
            bbox.inflate();
        }
};

#endif