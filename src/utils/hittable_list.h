/*
 * hittable_list.h
 *
 *  Created on: Apr. 24, 2023
 *      Author: alfred
 */

#ifndef UTILS_HITTABLE_LIST_H_
#define UTILS_HITTABLE_LIST_H_

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
	hittable_list() {};
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;


public:
	std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit (const ray& r, double tmin, double tmax, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = tmax;

	for (const auto& object : objects) {
		if (object-> hit(r, tmin, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

bool hittable_list::bounding_box(double t0, double t1, aabb& output_box) const {
	if (objects.empty()) return false;

	bool first = true;
	aabb temp_box;

	for (const auto& object : objects) {
		bool has_box = object->bounding_box(t0, t1, temp_box);
		if (!has_box) return false;
		output_box = first ? temp_box : surrounding_box(output_box, temp_box);
		first = false;
	}
	return true;
}

#endif /* UTILS_HITTABLE_LIST_H_ */
