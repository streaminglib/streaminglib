//
// Created by jin on 2018/5/7.
//

#ifndef STREAMINGLIB_BLOOM_FILTER_H
#define STREAMINGLIB_BLOOM_FILTER_H

#include <string>
#include <cstdlib>

#include "hash.h"
#include "hashtable.h"

using std::string;

class SpectralBloomFilter_Base
{
    friend class SpectralBloomFilter_RM; // 使 RM 能够访问私有的 hashtable
public:
    // Hash 由调用者提供，也可以考虑增加一个工厂方法为调用者设定 Hash 对象的参数
    SpectralBloomFilter_Base(size_t cell_width, size_t cells, Hash &hash);
    void insert_element(const string &elem);
    void delete_element(const string &elem);
    size_t query_element(const string &elem) const;
private:
    Hashtable hashtable;
    Hash& hash;
    size_t cells;
    size_t cell_width;
};

class SpectralBloomFilter_MI
{
public:
    SpectralBloomFilter_MI(size_t cell_width, size_t cells, Hash &hash);
    void insert_element(const string &elem);
    size_t query_element(const string &elem) const;
private:
    Hashtable hashtable;
    Hash& hash;
    size_t cells;
    size_t cell_width;
};

class SpectralBloomFilter_RM
{
public:
    // l1_XXX 为提供给 L1_SBF 的构造参数，l2_XXX 类似
    SpectralBloomFilter_RM(size_t l1_cell_width, size_t l2_cell_width,
                           size_t l1_cells, size_t l2_cells,
                           Hash &l1_hash, Hash &l2_hash);
    void insert_element(const string &elem);
    size_t query_element(const string &elem) const;
private:
    // 双层的 BloomFilter
    SpectralBloomFilter_Base L1_SBF;
    SpectralBloomFilter_Base L2_SBF;
};

#endif //STREAMINGLIB_BLOOM_FILTER_H
