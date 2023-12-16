#pragma once
#include <iostream>
#include <cmath>
#include "field/field.h"
#include <cassert>
using namespace std;

class Mat
{
public:
    ModInt m, n; // 行数和列数
    vector<vector<ModInt>> mat;

    Mat() {}
    Mat(ModInt m, ModInt n);
    Mat(vector<vector<ModInt>> &src);
    Mat(vector<vector<uint64_t>> &src);
    void create();
    void Print();                            // 打印矩阵
    void eye();                              // 令矩阵行列下标相同时值为 1，不同时为 0
    static bool inv(Mat src, Mat &src_inv); // 求矩阵的逆
};
