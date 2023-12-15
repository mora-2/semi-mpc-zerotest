#pragma once
#include <cstdint>
#include <random>

class ModInt
{
public:
    using FP = uint64_t;
    // static const FP fieldPrime = 2147483647; // const fieldPrime = 2147483647 // 2^31-1, 31 bits
    static const FP fieldPrime = 23;
    static const FP fieldPrime_bitlength = 5;
    FP value_;

private:
public:
    ModInt() : value_(0) {}
    explicit ModInt(FP value) : value_(value % fieldPrime) {}

    inline ModInt operator+(const ModInt &other) const
    {
        return ModInt((this->value_ + other.value_) % fieldPrime);
    }
    inline ModInt operator-(const ModInt &other) const
    {
        if (this->value_ < other.value_)
            return ModInt(this->value_ + fieldPrime - other.value_);
        else
            return ModInt(this->value_ - other.value_);
    }

    inline ModInt operator-() const
    {
        if (this->value_ != 0)
            return ModInt(fieldPrime - this->value_);
        else
            return ModInt(0);
    }

    inline ModInt operator*(const ModInt &other) const
    {
        ModInt::FP ans = 0;
        ModInt::FP x(this->value_), y(other.value_);

        while (y)
        {
            if (y & 1)
                ans = (ans + x) % fieldPrime;
            x = x * 2 % fieldPrime;
            y >>= 1;
        }
        return ModInt(ans);
    }

    inline ModInt &operator=(const ModInt &other)
    {
        if (this != &other)
        {
            value_ = other.value_;
        }
        return *this;
    }

    inline bool operator==(const ModInt &other)
    {
        return value_ == other.value_;
    }

    inline bool operator<(const ModInt &other)
    {
        return this->value_ < other.value_;
    }

    inline ModInt Inv()
    {
        FP ans = 1;
        FP value = this->value_;
        for (FP i = fieldPrime - 2; i; i >>= 1)
        {
            if (i & 1)
                ans = (value * ans) % fieldPrime;
            value = (value * value) % fieldPrime;
        }
        return ModInt(ans);
    }

    static inline ModInt Pow(ModInt num_, ModInt pow_)
    {
        FP num = num_.value_;
        FP pow = pow_.value_;
        FP result;
        for (result = 1; pow; pow >>= 1)
        {
            if (pow & 1)
                result = (result * num) % fieldPrime;
            num = (num * num) % fieldPrime;
        }
        return ModInt(result);
    }

    static inline ModInt Mod(FP a)
    {
        return ModInt(a);
    }

    static inline ModInt RandomFieldElement()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<FP> dis(0, ModInt::fieldPrime - 1);
        return ModInt(dis(gen));
    }

    ~ModInt() = default;
};

// Outside the ModInt class for googletest
inline bool operator==(const ModInt &lhs, const ModInt &rhs)
{
    return lhs.value_ == rhs.value_;
}