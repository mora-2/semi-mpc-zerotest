#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "field/field.h"
#include "field/mat.h"
#include <cassert>

std::vector<ModInt> MakeShare(ModInt number, int num_shares, std::mt19937 &gen);
double lagrangeInterpolation(const std::vector<double> &x, const std::vector<double> &y, double target);
ModInt lagrangeInterpolation(const std::vector<ModInt> &x, const std::vector<ModInt> &y, ModInt target);
std::vector<ModInt> LagrangeCoefficients(const ModInt::FP prime_bitlength);
