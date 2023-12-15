#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "utils/utils.h"
#include "field/field.h"
#include "third_party/eigen-3.4.0/Eigen/Dense"
#include <cmath>
#include "zerotest.h"

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

TEST(DISABLED_LagrangeInterpolation, coefficient1)
{
    std::vector<double> coeff = LagrangeCoefficients(ModInt::fieldPrime_bitlength);
    int i = 0;
    for (const double &value : coeff)
    {
        // cout << "coeff[" << i++ << "]:" << value << endl;
    }

    for (int i = 0; i < ModInt::fieldPrime_bitlength + 1; i++)
    {
        double coeff_value = 0;
        for (int j = 0; j < coeff.size(); j++)
        {

            coeff_value += coeff[j] * std::pow(i, j);
            // cout << "coeff_value:" << coeff_value << "\tcoeff[j]:" << coeff[j] << "\tstd::pow(i, j)" << std::pow(i, j) << endl;
        }
        if (i == 0)
        {

            GTEST_ASSERT_EQ(round(coeff_value), 0);
        }
        else
        {
            GTEST_ASSERT_EQ(round(coeff_value), 1);
        }
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

TEST(DISABLED_Eigen, eigen)
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 3;
    m(1, 0) = 2.5;
    m(0, 1) = -1;
    m(1, 1) = m(1, 0) + m(0, 1);
    std::cout << m << std::endl;
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
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}