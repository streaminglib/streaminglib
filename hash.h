//
// Created by jin on 2018/5/7.
//

#ifndef STREAMINGLIB_HASH_H
#define STREAMINGLIB_HASH_H

#include <vector>
#include <string>
#include "hashtable.h"
#include "MurmurHash3.h"
#include "CityHash.h"

using std::string;

class Hash
{
public:
    virtual nvec operator () (const string &s) const = 0;
};

class Murmur3Hash: public Hash {
public:
    Murmur3Hash(nvec seeds);
    nvec operator()(const string &s) const override;
private:
    nvec seeds;
};

class BinaryHash: public Hash 
{
public:
    BinaryHash(Hash &hash);
    nvec operator()(const string &s) const override;
private:
    Hash &hash;
};

class CityHash: public Hash
{
public:
    CityHash(nvec seeds);
    nvec operator()(const string &s) const override;
private:
    nvec seeds;
};

#endif //STREAMINGLIB_HASH_H
