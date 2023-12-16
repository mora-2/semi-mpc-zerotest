#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "utils/utils.h"
#include "field/field.h"
#include <cmath>
#include "zerotest.h"
#include "field/mat.h"

using namespace std;
TEST(DISABLED_LagrangeInterpolation, lagrangeInterpolation)
{
    // 提供一些数据点
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {2, 3, 1, 4, 2};
    // 计算拉格朗日插值
    int i = 0;
    for (double value : x)
    {
        double interpolatedValue = lagrangeInterpolation(x, y, value);
        GTEST_ASSERT_EQ(interpolatedValue, y[i++]);
        // // 输出结果
        // std::cout << "Interpolated value at " << value << " is: " << interpolatedValue << "\torigin:" << y[i++] << std::endl;
    }
}

TEST(DISABLED_LagrangeInterpolation, ModInt_lagrangeInterpolation)
{
    // 提供一些数据点
    std::vector<ModInt> x = {ModInt(1), ModInt(2), ModInt(3), ModInt(4), ModInt(5), ModInt(6)};
    std::vector<ModInt> y = {ModInt(0), ModInt(1), ModInt(1), ModInt(1), ModInt(1), ModInt(1)};
    // 计算拉格朗日插值
    int i = 0;
    for (ModInt value : x)
    {
        ModInt interpolatedValue = lagrangeInterpolation(x, y, value);
        GTEST_ASSERT_EQ(interpolatedValue.value_, y[i++].value_);
        // // 输出结果
        // std::cout << "Interpolated value at " << value << " is: " << interpolatedValue << "\torigin:" << y[i++] << std::endl;
    }
}

TEST(DISABLED_LagrangeInterpolation, coefficient)
{
    // 提供一些数据点
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {0, 1, 1, 1, 1};

    std::vector<double> coeff = LagrangeCoefficients(x, y);
    int i = 0;
    for (const double &value : coeff)
    {
        // cout << "coeff[" << i++ << "]:" << value << endl;
    }

    i = 0;
    for (const int &input : x)
    {
        double coeff_value = 0;
        for (int j = 0; j < coeff.size(); j++)
        {

            coeff_value += coeff[j] * std::pow(input, j);
            // cout << "coeff_value:" << coeff_value << "\tcoeff[j]:" << coeff[j] << "\tstd::pow(input, j)" << std::pow(input, j) << endl;
        }
        GTEST_ASSERT_EQ(round(coeff_value), y[i++]);
    }
}

TEST(DISABLED_LagrangeInterpolation, Mod_coefficient)
{
    std::vector<ModInt> x = {ModInt(1), ModInt(2), ModInt(3), ModInt(4), ModInt(5)};
    std::vector<ModInt> y = {ModInt(0), ModInt(1), ModInt(1), ModInt(1), ModInt(1)};

    std::vector<ModInt> coeff = LagrangeCoefficients(ModInt::fieldPrime_bitlength); // x, y set in function
    int i = 0;
    for (const ModInt &value : coeff)
    {
        cout << "coeff[" << i++ << "]:" << value.value_ << endl;
    }

    i = 0;
    for (const ModInt &input : x)
    {
        ModInt coeff_value = ModInt(0);
        for (int j = 0; j < coeff.size(); j++)
        {

            coeff_value = coeff_value + coeff[j] * ModInt::Pow(input, ModInt(j));
            cout << "coeff_value:" << coeff_value.value_ << "\tcoeff[j]:" << coeff[j].value_ << "\tstd::pow(input, j)" << ModInt::Pow(input, ModInt(j)).value_ << endl;
        }
        GTEST_ASSERT_EQ(coeff_value.value_, y[i++].value_);
    }
}

TEST(DISABLED_ModInt, test1)
{
    ModInt a(10);
    ModInt b(12);
    ModInt c(7);

    GTEST_ASSERT_EQ((a + b + c).value_, ModInt((10 + 12 + 7) % ModInt::fieldPrime).value_);
    GTEST_ASSERT_EQ((a + b - c).value_, ModInt((10 + 12 - 7) % ModInt::fieldPrime).value_);
}

TEST(DISABLED_ModInt, test2)
{
    ModInt a(10);
    ModInt b(12);
    ModInt c(7);

    a = a + b + c;
    GTEST_ASSERT_EQ(a.value_, ModInt((10 + 12 + 7) % ModInt::fieldPrime).value_);
}

TEST(DISABLED_ModInt, test3)
{
    ModInt a(11);
    cout << "a(11).Inv():" << a.Inv().value_ << endl;

    cout << (ModInt(2) * ModInt(12)).value_ << endl;
    cout << (ModInt(19) * ModInt(19).Inv()).value_ << endl;
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<ModInt::FP> dis(1, ModInt::fieldPrime - 1);

    for (int i = 0; i < 100; i++)
    {
        ModInt x(dis(g));
        cout << "x:" << x.value_ << "\tx.Inv():" << x.Inv().value_ << "\tx * x.Inv():" << (x * x.Inv()).value_ << endl;
        GTEST_ASSERT_EQ((x * x.Inv()).value_, ModInt(1).value_);
    }
}

TEST(DISABLED_ModInt, test4)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<ModInt::FP> dis(1, ModInt::fieldPrime - 1);

    for (int i = 0; i < 100; i++)
    {
        ModInt x(dis(g));
        ModInt y(dis(g));
        ModInt result(1);
        for (int i = 0; i < y.value_; i++)
            result = result * x;
        // cout << "x:" << x.value_ << "\ty:" << y.value_ << "\tx*y:" << result.value_ << endl;
        GTEST_ASSERT_EQ(ModInt::Pow(x, y).value_, result.value_);
    }
}

TEST(DISABLED_Mat, Inv)
{
    ModInt m(3);
    ModInt n(3);

    vector<vector<uint64_t>> a = {{1, 2, 3}, {3, 7, 2}, {8, 3, 9}};
    Mat src(a);

    Mat src_inv;
    if (Mat::inv(src, src_inv))
        src_inv.Print();

    for (int i = 0; i < m.value_; i++)
    {
        for (int j = 0; j < n.value_; j++)
        {
            ModInt tmp(0);
            for (int k = 0; k < m.value_; k++)
            {
                tmp = tmp + (src.mat[i][k] * src_inv.mat[k][j]);
            }
            if (i == j)
                GTEST_ASSERT_EQ(tmp.value_, 1);
            else
                GTEST_ASSERT_EQ(tmp.value_, 0);
        }
    }
}

/* set  Zerotest::SecretMul public*/
TEST(DISABLED_ZeroTest, SecretMul)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<ModInt::FP> dis(1, ModInt::fieldPrime - 1);
    for (int i = 0; i < 100; i++)
    {
        ModInt x(dis(g));
        ModInt y(dis(g));

        Zerotest zerotest(3);
        zerotest.Share(x, "x");
        zerotest.Share(y, "y");
        zerotest.SecretMul("x", "y", "x*y");
        ModInt result = zerotest.Reveal("x*y");
        // cout << "result:" << result.value_ << endl;
        GTEST_ASSERT_EQ(result.value_, (x * y).value_);
    }
}

TEST(DISABLED_ZeroTest, RandExpShareCPs)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<ModInt::FP> dis(1, ModInt::fieldPrime - 1);
    for (int i = 0; i < 100; i++)
    {
        ModInt x(dis(g));
        ModInt y(dis(g));

        Zerotest zerotest(2);
        zerotest.RandExpShareCPs("R", ModInt(ModInt::fieldPrime_bitlength));

        vector<ModInt> Rs;
        Rs.push_back(zerotest.Reveal("R^-1"));
        Rs.push_back(zerotest.Reveal("R")); // Rs[1] == R
        for (int j = 2; j < ModInt::fieldPrime_bitlength + 1; j++)
        {
            string R = "R^" + to_string(j);
            Rs.push_back(zerotest.Reveal(R));
        }

        for (int j = 0; j < ModInt::fieldPrime_bitlength; j++)
        {
            if (j == 0)
                GTEST_ASSERT_EQ((Rs[j] * Rs[1]).value_, ModInt(1).value_);
            else
                GTEST_ASSERT_EQ((Rs[j] * Rs[1]).value_, Rs[j + 1].value_);
        }
    }
}

TEST(ZeroTest, zerotest)
{
    for (int i = 0; i < ModInt::fieldPrime; i++)
    {
        Zerotest zerotest(2);
        ModInt result = zerotest.Testing("test", ModInt(i));
        if (i == 0)
            GTEST_ASSERT_EQ(result.value_, 0);
        else
            GTEST_ASSERT_EQ(result.value_, 1);
    }
}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}