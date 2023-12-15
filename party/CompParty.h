#pragma once
#include <map>
#include "field/field.h"
#include <deque>

using namespace std;
class CompParty
{
public:
    // party ID
    uint64_t id;
    // offline pre-process
    map<string, ModInt> shares;
    map<string, ModInt> reveals;
    deque<array<ModInt, 3>> beaver; // for secret multiply

private:
public:
    CompParty(uint64_t id) : id(id) {}
    void MergeAdd(string var1, string var2, string new_var);
    ~CompParty() = default;
};