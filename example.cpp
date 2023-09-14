/*
 * File: example.cpp
 * Author: Julian Schilliger
 * 
 * Created on: September 2023
 *
 * Description: Example usage of the CubicMultithreadedSpline class.
 *
 * License: MIT License
 *
 * Copyright (c) 2023 Julian Schilliger
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Compilation: g++ -std=c++17 -g -o example example.cpp CubicMultithreadedSpline.cpp -I /usr/include/eigen3 -I/usr/include/python3.8 -I/usr/include/opencv4 -lpython3.8 -lgsl -lgslcblas -lpthread -lopencv_core
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <gsl/gsl_integration.h>
#include "matplotlibcpp.h"
#include <algorithm>
#include <cmath>
#include <utility>
#include <numeric>
#include <thread>
#include <mutex>
#include "CubicMultithreadedSpline.hpp"
#include <opencv2/core.hpp>

using Eigen::VectorXd;
using Eigen::MatrixXd;

using Voxel = cv::Vec3d;
using Pixel = cv::Vec2d;

namespace plt = matplotlibcpp;

// Example usage of the CubicMultithreadedSpline class
int main() {

    // Data generation
    Eigen::VectorXd x(15), y(15), range_xy(15);
    x << 0, 1.5, 2.3, 3, 4, 3.333, 2.1, 1, 0.5, 1.4, 2, 3, 4, 5, 6;
    y << 0, 0.5, 1.5, 3, 1.5, 1, 2, 3, 5, 3, 7, 9, 11, 13, 15;

    CubicMultithreadedSpline spline(x, y);

    // Generate t_values from 0 to 1
    std::vector<double> t_values(1000);
    std::iota(t_values.begin(), t_values.end(), 0);
    std::transform(t_values.begin(), t_values.end(), t_values.begin(), [](double t) { return t / 999.0; });

    std::vector<double> x_double_t_values, y_double_t_values;
    for (double t : t_values) {
        Pixel p_t = spline(t);
        double x_t = p_t[0];
        double y_t = p_t[1];
        x_double_t_values.push_back(x_t);
        y_double_t_values.push_back(y_t);
    }

    // Generate t_values from 0 to 1
    int number_spaced = 40;
    std::vector<double> t_values_equally(number_spaced);
    std::iota(t_values_equally.begin(), t_values_equally.end(), 0);
    std::transform(t_values_equally.begin(), t_values_equally.end(), t_values_equally.begin(), [number_spaced](double t) { return t / ((double)number_spaced - 1.0); });

    std::vector<double> x_double_t_values_equally, y_double_t_values_equally;
    for (double t : t_values_equally) {
        Pixel p_t = spline(t);
        double x_t_equally = p_t[0];
        double y_t_equally = p_t[1];
        x_double_t_values_equally.push_back(x_t_equally);
        y_double_t_values_equally.push_back(y_t_equally);
    }

    std::map<std::string, std::string> keywords;
    keywords["color"] = "red";
    std::map<std::string, std::string> keywords_equally;
    keywords_equally["color"] = "green";
    plt::title("Double Spline evaluated at t");
    plt::plot(x_double_t_values, y_double_t_values);  // Replace these vectors with what you've computed
    plt::scatter(std::vector<double>(x_double_t_values_equally.data(), x_double_t_values_equally.data() + x_double_t_values_equally.size()),
                 std::vector<double>(y_double_t_values_equally.data(), y_double_t_values_equally.data() + y_double_t_values_equally.size()),
                 8.0, keywords_equally);
    plt::scatter(std::vector<double>(x.data(), x.data() + x.size()),
                 std::vector<double>(y.data(), y.data() + y.size()),
                 10.0, keywords);


    plt::show();
    return 0;
}