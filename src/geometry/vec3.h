/*
 * vec3.h
 *
 *  Created on: Apr. 21, 2023
 *      Author: alfred
 */

#ifndef GEOMETRY_VEC3_H_
#define GEOMETRY_VEC3_H_


#include <cmath>
#include <iostream>

#include "../utils/utility.h"

using std::sqrt;

class vec3 {
public:
	// Constructors
	vec3() : e{0, 0, 0} {}
	vec3(double e1, double e2, double e3) : e{e1, e2, e3} {}

	// Functions
	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	/**
	 * Returns the pointwise min of v1 and v2.
	 * ex. min((1, 2, 3), (-1, 3, -2)) = (-1, 2, -2)
	*/
	static vec3 min(const vec3& v1, const vec3& v2) {
		return vec3(
			std::fmin(v1.x(), v2.x()),
			std::fmin(v1.y(), v2.y()),
			std::fmin(v1.z(), v2.z())
		);
	}

	/**
	 * Returns the pointwise max of v1 and v2.
	 * ex. max((1, 2, 3), (-1, 3, -2)) = (1, 3, 3)
	*/
	static vec3 max(const vec3& v1, const vec3& v2) {
		return vec3(
			std::fmax(v1.x(), v2.x()),
			std::fmax(v1.y(), v2.y()),
			std::fmax(v1.z(), v2.z())
		);
	}

	// Operations
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline double operator[](int i) const { return e[i]; }
	inline double& operator[](int i) { return e[i]; }

	inline vec3& operator+=(const vec3& other) {
		e[0] += other.e[0];
		e[1] += other.e[1];
		e[2] += other.e[2];
		return *this;
	}

	inline vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	inline vec3& operator/=(const double t) {
		e[1] /= t;
		e[0] /= t;
		e[2] /= t;
		return *this;
	}

	inline double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline double length() const {
		return sqrt(static_cast<float>(length_squared()));
	}

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (std::fabs(static_cast<float>(e[0])) < s)
        		&& (std::fabs(static_cast<float>(e[1])) < s)
				&& (std::fabs(static_cast<float>(e[2])) < s);
    }

public:
	double e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color


// Utility functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2] << "\n";
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator*(double t, const vec3& v) {
	return v * t;
}

inline vec3 operator/(const vec3& v, double t) {
	return (1/t) * v;
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
	return v1 + (-v2);
}

inline double dot(const vec3& v1, const vec3& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
	double x = v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1];
	double y = v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2];
	double z = v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0];
	return vec3(x, y, z);
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& uv, const vec3& n, double ir_ratio) {
	// Assumes uv is a unit vector and that n is the face normal
	// i.e. dot(uv, n) < 0.
	double cos_theta = std::fmin(-dot(uv, n), 1.0);
	vec3 r_perp = ir_ratio * (uv + cos_theta * n);
	vec3 r_parallel = -std::sqrt(
			std::fabs(static_cast<float>(1.0 - r_perp.length_squared()))) * n;
	return r_perp + r_parallel;
}

vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

#endif /* GEOMETRY_VEC3_H_ */
