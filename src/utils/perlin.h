/*
* perlin.h
* created May 26, 2024
*/
#ifndef UTILS_PERLIN_H_
#define UTILS_PERLIN_H_

#include <cstdlib>
#include <cmath>
#include <vector>

#include "utility.h"

class perlin {
    private: 
        int n;
        std::vector<std::vector<double>> x_grad;
        std::vector<std::vector<double>> y_grad;

        /**
        * Takes (x, y) in [0, 1]^2 and returns which of the perlin grid
        * squares it belongs to. 
        */
        std::vector<int> get_grid_square(double x, double y) const {
            int row = int(x * n);
            int col = int(y * n);
            std::vector<int> coordinates = {row, col};
            return coordinates;
        }

        inline double smoother_step(double x) const {
            return (x * (6*x - 15.0) + 10.0) * x * x * x;
        }

    public:
        perlin(int num_points) {
            n = num_points;
            x_grad = std::vector<std::vector<double>>(n, std::vector<double>(n, 0.0));
            y_grad = std::vector<std::vector<double>>(n, std::vector<double>(n, 0.0));

            // initialize each corner of the grid with a random value, and a random unit gradient vector
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    double theta = random_double(0.0, 2 * pi);
                    x_grad[i][j] = std::cos(theta);
                    y_grad[i][j] = std::sin(theta);
                }
            }
        }

        /**
         * Returns noise given texture coordinates (u, v).
        */
        double noise(double u, double v) const {
            auto coordinates = get_grid_square(u, v);
            int row = coordinates[0]; int col = coordinates[1];

            // Normalize (u, v) so that they lie in the n x n grid space
            u *= n; v *= n;

            double du = u - (double) row;
            double dv = v - (double) col;

            double iu = smoother_step(du);
            double iv = smoother_step(dv);

            double noise_value = 0.0;

            noise_value += (1-iu) * (1-iv) * (-du * x_grad[row][col] + -dv * y_grad[row][col]);
            noise_value += (1-iu) * iv * (-du * x_grad[row][col+1] + (1-dv) * y_grad[row][col+1]);
            noise_value += iu * (1-iv) * ((1-du)*x_grad[row+1][col] + -dv * y_grad[row+1][col]);
            noise_value += iu * iv * ((1-du)*x_grad[row+1][col+1] + (1-dv)*y_grad[row+1][col+1]);

            return noise_value;
        }

};

#endif