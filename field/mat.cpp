#include "mat.h"

using namespace std;

Mat::Mat(ModInt m, ModInt n)
{
    this->m = m;
    this->n = n;
    this->mat.resize(m.value_);
    for (vector<ModInt> &col : this->mat)
    {
        col.resize(n.value_);
    }
}

Mat::Mat(vector<vector<ModInt>> &src)
{
    this->m = ModInt(src.size());
    this->n = ModInt(src[0].size());
    this->mat.resize(m.value_);
    for (vector<ModInt> &col : this->mat)
    {
        col.resize(n.value_);
    }

    for (int i = 0; i < m.value_; i++)
    {
        for (int j = 0; j < n.value_; j++)
        {
            this->mat[i][j] = src[i][j];
        }
    }
}

Mat::Mat(vector<vector<uint64_t>> &src)
{
    this->m = ModInt(src.size());
    this->n = ModInt(src[0].size());
    this->mat.resize(m.value_);
    for (vector<ModInt> &col : this->mat)
    {
        col.resize(n.value_);
    }

    for (int i = 0; i < m.value_; i++)
    {
        for (int j = 0; j < n.value_; j++)
        {
            this->mat[i][j] = ModInt(src[i][j]);
        }
    }
}

void Mat::create()
{
    for (int i = 0; i < m.value_; i++)
    {
        for (int j = 0; j < n.value_; j++)
        {
            int tmp = 0;
            cin >> tmp;
            mat[i][j] = ModInt(tmp);
        }
    }
}

void Mat::Print()
{
    for (int i = 0; i < m.value_; i++)
    {
        for (int j = 0; j < n.value_; j++)
        {
            cout << mat[i][j].value_ << "\t";
        }
        cout << endl;
    }
}
void Mat::eye()
{
    for (int i = 0; i < m.value_; i++)
    {
        for (int j = 0; j < n.value_; j++)
        {
            if (i == j)
                mat[i][j] = ModInt(1);
            else
                mat[i][j] = ModInt(0);
        }
    }
}
bool Mat::inv(Mat src, Mat &src_inv)
{
    if (src.n.value_ != src.m.value_)
    {
        cout << "不可逆！" << endl;
        return false;
    }
    src_inv.m = src.m;
    src_inv.n = src.n;
    src_inv.mat.resize(src_inv.m.value_);
    for (vector<ModInt> &col : src_inv.mat)
    {
        col.resize(src_inv.n.value_);
    }
    src_inv.eye(); // 创建单位矩阵

    // 下来进行自上而下的初等行变换，使得矩阵 src.mat 变成单位上三角矩阵
    for (int i = 0; i < src_inv.m.value_; i++) // 注意这里要 i<=m，和之前的上三角矩阵有不同
    {                                          // 因为要判断最后一行化为上三角矩阵的最后一行最后一列元素是否为 0
        // 寻找第 i 列不为零的元素
        int k;
        for (k = i; k <= src_inv.m.value_; k++)
        {
            if (src.mat[k][i].value_ > 0) // 满足这个条件时，认为这个元素不为0
                break;
        }
        if (k <= src_inv.m.value_) // 说明第 i 列有不为0的元素
        {
            if (k != i) // 说明第 i 行 第 i 列元素为零，需要和其他行交换
            {
                // 交换第 i 行和第 k 行所有元素
                for (int j = 0; j < src_inv.n.value_; j++) // 需从第一个元素交换，注意与之前化上三角矩阵不同
                {                                          // 使用mat[0][j]作为中间变量交换元素,两个矩阵都要交换
                    ModInt tmp = src.mat[i][j];
                    src.mat[i][j] = src.mat[k][j];
                    src.mat[k][j] = tmp;
                    tmp = src_inv.mat[i][j];
                    src_inv.mat[i][j] = src_inv.mat[k][j];
                    src_inv.mat[k][j] = tmp;
                }
            }
            ModInt b = src.mat[i][i]; // 倍数
            // 将矩阵 src.mat 的主对角线元素化为 1
            for (int j = 0; j < src_inv.n.value_; j++) // 从第一个元素开始
            {
                src.mat[i][j] = src.mat[i][j] * b.Inv();
                src_inv.mat[i][j] = src_inv.mat[i][j] * b.Inv();
            }
            for (int j = i + 1; j < src_inv.m.value_; j++) // ???
            {
                // 注意本来为 -src.mat[j][i]/src.mat[i][i],因为a.mat[i][i]等于 1，则不需要除它
                b = -src.mat[j][i];
                for (k = 0; k < src_inv.n.value_; k++)
                {
                    src.mat[j][k] = src.mat[j][k] + b * src.mat[i][k]; // 第 i 行 b 倍加到第 j 行
                    src_inv.mat[j][k] = src_inv.mat[j][k] + b * src_inv.mat[i][k];
                }
            }
        }
        else
        {
            cout << "不可逆！" << endl;
            return false;
        }
    }

    // 下面进行自下而上的行变换，将 src.mat 矩阵化为单位矩阵
    for (int i = src_inv.m.value_ - 1; i > 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            ModInt b = -src.mat[j][i];
            src.mat[j][i] = ModInt(0); // 实际上是通过初等行变换将这个元素化为 0,
            for (int k = 0; k < src_inv.n.value_; k++)
            { // 通过相同的初等行变换来变换右边矩阵
                src_inv.mat[j][k] = src_inv.mat[j][k] + b * src_inv.mat[i][k];
            }
        }
    }
    return true;
}
