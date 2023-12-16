#pragma once
#include <vector>
#include <random>
#include "field/field.h"
#include "party/CompParty.h"
#include "party/DataParty.h"
#include "utils/utils.h"
#include <cassert>

using namespace std;
class Zerotest
{
public:
    using PRG = std::mt19937;
    vector<CompParty> comp_party;
    DataParty data_party;
    ModInt testing_value;

    /* offline CPs data */
    int num_beaver;
    vector<ModInt> coeff;

private:
    std::random_device rd;
    PRG gen;

public:
    Zerotest() = default;
    Zerotest(uint64_t num_party);
    Zerotest &operator=(const Zerotest &other);
    ModInt Testing(string test_name, ModInt test_value);
    void Share(const ModInt &x, string var_name); // DP share [[x]] to CPs
    void Lookup(string var, string result_name);
    ModInt Reveal(const char *var);
    ModInt Reveal(string &var);
    void RevealCPs(string var); // reveal value to CPs
    ~Zerotest() = default;

private:
    void preprocess(int num_beaver);
    /* share: more need to be done*/
    void RandShareCPs(string var);           // CPs random value share
    ModInt Rand2ShareCPs(string var);        // CPs random {0, 1} share
    void ShareCPs(string var, ModInt value); // CPs share [[var]] = value
    /* secret multiply */
    void PrepareBeaver(uint32_t num_beaver); // beaver triple share

public: /* for test */
    void SecretMul(string var1, string var2, string result_name);
    void ScalerAdd(string var, ModInt scaler, string result_name);
    void ScalerAdd(string var, ModInt scaler);
    void RandExpShareCPs(string var, ModInt number);
};
