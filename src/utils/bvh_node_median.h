/*
 * bvh_node_median.h
 *
 *  Created on: Apr. 28, 2023
 *      Author: alfred
 */

#ifndef UTILS_BVH_NODE_MEDIAN_H_
#define UTILS_BVH_NODE_MEDIAN_H_


#include "hittable_list.h"
#include "../geometry/aabb.h"
#include "bvh_node.h"

#include <algorithm>
#include <numeric>
#include <iostream>

/**
 * bvh_node class taken from "Ray Tracing in One Week". Builds BVH tree using the median heuristic.
 */
class bvh_node_median : public hittable {
    public:
        bvh_node_median();

        bvh_node_median(const hittable_list& list, double time0, double time1)
            : bvh_node_median(list.objects, 0, list.objects.size(), time0, time1)
        {}

        bvh_node_median(
            const std::vector<shared_ptr<hittable>>& src_objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb box;
};

bool bvh_node_median::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box;
    return true;
}

bool bvh_node_median::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bvh_node_median::bvh_node_median(
    const std::vector<shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, double time0, double time1
) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
        left = make_shared<bvh_node_median>(objects, start, mid, time0, time1);
        right = make_shared<bvh_node_median>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (  !left->bounding_box (time0, time1, box_left)
       || !right->bounding_box(time0, time1, box_right)
    )
        std::cerr << "No bounding box in bvh_node_median constructor.\n";

    box = surrounding_box(box_left, box_right);
}

#endif /* UTILS_BVH_NODE_MEDIAN_H_ */
