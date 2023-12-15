#include "zerotest.h"

Zerotest::Zerotest(uint64_t num_party)
{
    /* init CPs */
    for (uint64_t i = 0; i < num_party; i++)
        this->comp_party.push_back(CompParty(i));

    /* init PRG */
    this->gen = PRG(rd());

    /* pre-process */
    this->num_beaver = 10;
    this->preprocess(num_beaver);
}

Zerotest &Zerotest::operator=(const Zerotest &other)
{
    // Remember to handle resources properly (e.g., if there are pointers)
    return *this;
}

void Zerotest::Testing()
{
    /* checking */
    assert(this->coeff.size() == ModInt::fieldPrime);
    for (CompParty &party : this->comp_party)
    {
        assert(party.shares.find("r") != party.shares.end());
        for (int i = 0; i < ModInt::fieldPrime; i++)
        {
            assert(party.shares.find("r_" + std::to_string(i)) != party.shares.end());
        }
    }

    string test_name = "test";
    this->testing_value = ModInt(0);
    this->Share(this->testing_value, test_name);

    /* [[m]] = [[r]] + [[testing_value]] */
    for (CompParty &party : this->comp_party)
    {
        party.shares["m"] = party.shares["r"] + party.shares[test_name];
    }

    /* reveal [[m]] */
    this->RevealCPs("m");

    /* [[1 + h]] & Lookup() */
    for (CompParty &party : this->comp_party)
    {
        party.shares["1+h"] = ModInt(1);
        for (int i = 0; i < ModInt::fieldPrime; i++)
        {
            ModInt m_i((party.reveals["m"].value_ >> i) & 1); // m_i
            party.shares["1+h"] += m_i + party.shares["r_" + std::to_string(i)] - 2 * m_i * party.shares["r_" + std::to_string(i)];
        } /* [[1+h]] */
        this->Lookup("1+h", "b");
    }
    cout << "result: " << this->Reveal("b").value_ << endl;
}

void Zerotest::Share(const ModInt &x, string var_name)
{
    this->RandShareCPs(var_name); // [[x]] for DP Share(x)
    ModInt a = this->Reveal(var_name);
    this->comp_party[0].shares[var_name] = this->comp_party[0].shares[var_name] + x - a; // x shared to CPs
}

void Zerotest::Lookup(string var, string result_name)
{
    for (CompParty &party : this->comp_party)
    {
        this->SecretMul("R^-1", var, "tmp_a"); // share "tmp_a"
    }
    this->RevealCPs("tmp_a");
    for (CompParty &party : this->comp_party)
    {
        for (int i = 2; i < ModInt::fieldPrime_bitlength + 1; i++)
        {
            party.shares[var + "^" + std::to_string(i)] = ModInt::Pow(party.reveals["tmp_a"], i) * party.shares["R^" + std::to_string(i)];
        }
    }
    for (CompParty &party : this->comp_party)
    {
        for (int i = 0; i < ModInt::fieldPrime_bitlength + 1; i++)
        {
            if (i == 0)
                party.shares[result_name] = party.shares[result_name] + this->coeff[i] * party.shares[var];
            else
                party.shares[result_name] = party.shares[result_name] + this->coeff[i] * party.shares[var + "^" + std::to_string(i)];
        }
    }

    for (CompParty &party : this->comp_party)
    {
        party.shares.erase("tmp_a");
        party.reveals.erase("tmp_a");
        for (int i = 2; i < ModInt::fieldPrime_bitlength + 1; i++)
        {
            party.shares.erase(var + "^" + std::to_string(i)); // [[x^i]]
        }
    }
}

ModInt Zerotest::Reveal(const char *var)
{
    ModInt result(0);
    for (CompParty &party : this->comp_party)
    {
        result = result + party.shares[var];
    }
    return result;
}

ModInt Zerotest::Reveal(string &var)
{
    ModInt result(0);
    for (CompParty &party : this->comp_party)
    {
        result = result + party.shares[var];
    }
    return result;
}

void Zerotest::RevealCPs(string var)
{
    ModInt value = this->Reveal(var);
    for (CompParty &party : this->comp_party)
    {
        party.reveals[var] = value; // insert
    }
}

void Zerotest::preprocess(int num_beaver)
{
    ModInt r(0);
    for (uint64_t i = 0; i < ModInt::fieldPrime_bitlength; i++)
    {
        ModInt ret = this->Rand2ShareCPs("r_" + std::to_string(i)); // [[r_i]] for [[1 + H]]
        r = r + ret * ModInt(pow(2, i));
    }
    this->ShareCPs("r", r); // [[r]]

    this->coeff = LagrangeCoefficients(ModInt::fieldPrime_bitlength); // coeff

    this->PrepareBeaver(this->num_beaver);
}

void Zerotest::RandShareCPs(string var)
{
    std::uniform_int_distribution<ModInt::FP> dis(1, ModInt::fieldPrime - 1);
    ModInt rand(dis(gen));

    // std::cout << "var name:" << var << "\trandom value:" << rand.value_ << std::endl;
    std::vector<ModInt> share = MakeShare(rand, this->comp_party.size(), gen);
    for (CompParty &party : this->comp_party)
    {
        party.shares.emplace(var, share[party.id]);
    }
}

ModInt Zerotest::Rand2ShareCPs(string var)
{
    std::uniform_int_distribution<ModInt::FP> dis(0, 1);
    ModInt rand(dis(gen));

    // std::cout << "var name:" << var << "\trandom value:" << rand.value_ << std::endl;
    std::vector<ModInt> share = MakeShare(rand, this->comp_party.size(), gen);
    for (CompParty &party : this->comp_party)
    {
        party.shares.emplace(var, share[party.id]);
    }
    return rand;
}

void Zerotest::ShareCPs(string var, ModInt value)
{
    // std::cout << "var name:" << var << "\tvalue:" << value.value_ << std::endl;
    std::vector<ModInt> share = MakeShare(value, this->comp_party.size(), gen);
    for (CompParty &party : this->comp_party)
    {
        party.shares.emplace(var, share[party.id]);
    }
}

void Zerotest::PrepareBeaver(uint32_t num_beaver)
{
    std::uniform_int_distribution<ModInt::FP> dis(0, ModInt::fieldPrime - 1);
    for (int i = 0; i < num_beaver; i++)
    {
        ModInt a(dis(gen));
        ModInt b(dis(gen));
        ModInt c = a * b;

        std::vector<ModInt> a_share = MakeShare(a, this->comp_party.size(), gen);
        std::vector<ModInt> b_share = MakeShare(b, this->comp_party.size(), gen);
        std::vector<ModInt> c_share = MakeShare(c, this->comp_party.size(), gen);
        for (int j = 0; j < this->comp_party.size(); j++)
        {
            this->comp_party[j].beaver.emplace_back(array<ModInt, 3>{a_share[j], b_share[j], c_share[j]});
        }
    }
}

void Zerotest::SecretMul(string var1, string var2, string result_name)
{
    assert(!this->comp_party[0].beaver.empty());
    /* alpha & beta*/
    for (int i = 0; i < this->comp_party.size(); i++)
    {
        ModInt alpha = this->comp_party[i].shares[var1] - this->comp_party[i].beaver.front()[0];
        this->comp_party[i].shares.emplace("alpha", alpha);
        ModInt beta = this->comp_party[i].shares[var2] - this->comp_party[i].beaver.front()[1];
        this->comp_party[i].shares.emplace("beta", beta);
    }
    this->RevealCPs("alpha");
    this->RevealCPs("beta");

    /* result */
    for (CompParty &party : this->comp_party)
    {
        ModInt result(0);
        if (party.id == 0)
        {
            result = party.beaver.front()[2] + party.reveals["alpha"] * party.beaver.front()[1] + party.reveals["beta"] * party.beaver.front()[0] + party.reveals["alpha"] * party.reveals["beta"];
        }
        else
        {
            result = party.beaver.front()[2] + party.reveals["alpha"] * party.beaver.front()[1] + party.reveals["beta"] * party.beaver.front()[0];
        }
        party.shares.emplace(result_name, result);

        /* delete */
        party.beaver.pop_front(); // beaver
        party.reveals.erase("alpha");
        party.reveals.erase("beta");
        party.shares.erase("alpha");
        party.shares.erase("beta");
    }
}

void Zerotest::RandExpShareCPs(string var, ModInt number_exps)
{
    ModInt tmp_a;
    string R = var;
    do
    {
        this->RandShareCPs(R);
        this->RandShareCPs("tmp_r");
        this->SecretMul(R, "tmp_r", "tmp_a");
        this->RevealCPs("tmp_a");
        tmp_a = this->Reveal("tmp_a");
    } while (tmp_a.value_ == 0);

    /* R^-1 */
    for (CompParty &party : this->comp_party)
    {
        ModInt R_Inv = party.reveals["tmp_a"].Inv() * party.shares["tmp_r"];
        party.shares[R + "^-1"] = R_Inv;
    }

    /* R^i */
    for (int i = 2; i < number_exps.value_ + 1; i++)
    {
        if (i - 1 == 1)
        {
            this->SecretMul(R, R, R + "^2");
        }
        else
        {
            this->SecretMul(R, R + "^" + std::to_string(i - 1), R + "^" + std::to_string(i));
        }
    }

    /* delete */
    for (CompParty &party : this->comp_party)
    {
        party.shares.erase("tmp_a");
        party.shares.erase("tmp_r");
        party.reveals.erase("tmp_a");
    }
}
