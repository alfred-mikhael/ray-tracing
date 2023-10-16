/*
 * bvh_node.h
 *
 *  Created on: Apr. 26, 2023
 *      Author: alfred
 */

#ifndef UTILS_BVH_NODE_H_
#define UTILS_BVH_NODE_H_

#include "hittable_list.h"
#include "../geometry/aabb.h"

#include <algorithm>
#include <numeric>
#include <iostream>

/**
 * Builds a BVH tree using the surface area heuristic with binning.
 */
class bvh_node : public hittable {
    public:
        bvh_node();

        bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size() - 1, time0, time1)
        {}

        bvh_node(
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

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box;
    return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0,0, box_a) || !b->bounding_box(0,0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}


bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

double evaluate(const std::vector<shared_ptr<hittable>>& obs, int axis, int split,
		size_t start, size_t end, double t0, double t1) {
	int num_left(0), num_right(0);
	aabb left_box, right_box;
	left_box = right_box = aabb(point3(0, 0, 0), point3(0, 0, 0));

	aabb cutoff_box;
	obs[split]->bounding_box(t0, t1, cutoff_box);
	double cutoff = cutoff_box.max().e[axis] + cutoff_box.min().e[axis];

	for (size_t i = start; i < end; i++) {
		aabb temp_box;
		obs[i]->bounding_box(t0, t1, temp_box);
		if (temp_box.max().e[axis] + temp_box.min().e[axis] <= cutoff) {
			num_left++;
			left_box = surrounding_box(left_box, temp_box);
		} else {
			num_right++;
			right_box = surrounding_box(right_box, temp_box);
		}
	}
	return left_box.surface_area() * num_left + right_box.surface_area() * num_right;
}

bvh_node::bvh_node(
    const std::vector<shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, double t0, double t1
) {
    auto objects = src_objects;

    size_t object_span = end - start;

    if (object_span == 0) {
        left = right = objects[start];
    } else if (object_span == 1) {
    	left = objects[start];
    	right = objects[start + 1];
    } else {
    	int best_axis = -1;
    	int best_split = -1;
    	double best_cost = std::numeric_limits<double>::max();
    	// Find best split by finding the best split position out of n possible planes in each axis.
    	for (int a = 0; a < 3; a++) {
    		auto comparator = (a == 0) ? box_x_compare : (a == 1) ? box_y_compare : box_z_compare;
            std::sort(objects.begin() + start, objects.begin() + end + 1, comparator);
            for (int i = 0; i < 100; i++) {
            	int k = random_int(start, end-1);
            	double split_cost = evaluate(objects, a, k, start, end, t0, t1);
            	if (split_cost < best_cost) {
            		best_cost = split_cost;
            		best_axis = a;
            		best_split = k;
            	}
            }
    	}
    	auto comparator = (best_axis == 0) ? box_x_compare : (best_axis == 1) ? box_y_compare : box_z_compare;
    	std::sort(objects.begin() + start, objects.begin() + end + 1, comparator);
    	// Recurse to set the left and right children
        left = make_shared<bvh_node>(objects, start, best_split, t0, t1);
        right = make_shared<bvh_node>(objects, best_split + 1, end, t0, t1);
    }

    aabb box_left, box_right;

    if (  !left->bounding_box (t0, t1, box_left)
       || !right->bounding_box(t0, t1, box_right)
    )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surrounding_box(box_left, box_right);
}

#endif /* UTILS_BVH_NODE_H_ */
