#include "CompParty.h"

void CompParty::MergeAdd(string var1, string var2, string new_var)
{
    this->shares.emplace(new_var, shares[var1] + shares[var2]);
    this->shares.erase(var1);
    this->shares.erase(var2);
}