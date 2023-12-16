#include "utils.h"

using namespace std;

std::vector<ModInt> MakeShare(ModInt number, int num_shares, std::mt19937 &gen)
{
    std::uniform_int_distribution<ModInt::FP> dis(1, ModInt::fieldPrime - 1);
    std::vector<ModInt> partitions;

    ModInt::FP last_share = 0;
    for (int i = 0; i < num_shares - 1; i++)
    {
        ModInt::FP tmp = dis(gen);
        partitions.push_back(ModInt(tmp));
        last_share = (last_share + tmp) % ModInt::fieldPrime;
    }
    partitions.push_back(ModInt(number - ModInt(last_share)));
    return partitions;
}

double lagrangeInterpolation(const std::vector<double> &x, const std::vector<double> &y, double target)
{
    if (x.size() != y.size() || x.empty())
    {
        std::cerr << "Invalid input: x and y must have the same non-zero size." << std::endl;
        return 0.0;
    }

    double result = 0.0;

    for (std::size_t i = 0; i < x.size(); ++i)
    {
        double term = y[i];

        for (std::size_t j = 0; j < x.size(); ++j)
        {
            if (j != i)
            {
                term *= (target - x[j]) / (x[i] - x[j]);
            }
        }

        result += term;
    }

    return result;
}

ModInt lagrangeInterpolation(const std::vector<ModInt> &x, const std::vector<ModInt> &y, ModInt target)
{
    if (x.size() != y.size() || x.empty())
    {
        std::cerr << "Invalid input: x and y must have the same non-zero size." << std::endl;
        return ModInt(0);
    }

    ModInt result(0);

    for (std::size_t i = 0; i < x.size(); ++i)
    {
        ModInt term = y[i];

        for (std::size_t j = 0; j < x.size(); ++j)
        {
            if (j != i)
            {
                term = term * ((target - x[j]) * (x[i] - x[j]).Inv());
            }
        }

        result = result + term;
    }

    return result;
}

std::vector<double> LagrangeCoefficients(const std::vector<double> &x, const std::vector<double> &y)
{
    Eigen::MatrixXd A(ModInt::fieldPrime_bitlength + 1, ModInt::fieldPrime_bitlength + 1);
    Eigen::VectorXd B(ModInt::fieldPrime_bitlength + 1);

    // set A
    for (int i = 0; i < ModInt::fieldPrime_bitlength + 1; i++)
    {
        for (int j = 0; j < ModInt::fieldPrime_bitlength + 1; j++)
        {
            A(i, j) = pow(x[i], j);
        }
    }
    // set B
    for (int i = 0; i < ModInt::fieldPrime_bitlength + 1; i++)
    {
        B(i) = y[i];
    }

    // solution
    Eigen::VectorXd X = A.colPivHouseholderQr().solve(B);
    std::vector<double> ret;
    for (int i = 0; i < ModInt::fieldPrime_bitlength + 1; i++)
    {

        ret.push_back(X(i));
    }
    // std::cout << "X:" << X << std::endl;

    return ret;
}

// std::vector<ModInt> LagrangeCoefficients(const std::vector<ModInt> &x, const std::vector<ModInt> &y);
// {

// }

/*
    x = {1, 2, ..., ModInt::fieldPrime_bitlength + 1}
    y = {0, 1, ..., 1}
 */
std::vector<ModInt> LagrangeCoefficients(const ModInt::FP prime_bitlength)
{
    vector<vector<ModInt>> A(prime_bitlength + 1, std::vector<ModInt>(prime_bitlength + 1));
    vector<ModInt> B(prime_bitlength + 1);

    // set A
    for (int i = 0; i < prime_bitlength + 1; i++)
    {
        for (int j = 0; j < prime_bitlength + 1; j++)
        {
            A[i][j] = ModInt::Pow(ModInt(i + 1), ModInt(j));
        }
    }
    // set B
    for (int i = 0; i < prime_bitlength + 1; i++)
    {
        if (i == 0)
            B[i] = ModInt(0);
        else
            B[i] = ModInt(1);
    }

    Mat mat_A(A);
    Mat inv_A;
    Mat::inv(mat_A, inv_A);

    std::vector<ModInt> ret(prime_bitlength + 1, ModInt(0));

    for (int i = 0; i < prime_bitlength + 1; i++)
    {
        for (int j = 0; j < prime_bitlength + 1; j++)
        {
            ret[i] = ret[i] + inv_A.mat[i][j] * B[j];
        }
    }

    return ret;
}
