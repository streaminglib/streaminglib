//
// Created by jin on 2018/5/8.
//

#ifndef STREAMINGLIB_SKETCH_H
#define STREAMINGLIB_SKETCH_H

#include "hash.h"
#include "hashtable.h"

class CMSketch
{
public:
    CMSketch(size_t rows, size_t cell_width, size_t cells, Hash &hash);
    void insert_element(const string &elem, size_t delta=1);
    size_t query_element(const string &elem) const;
    size_t query_inner_product(const CMSketch &cms) const;
private:
    Hashtable hashtable;
    Hash hash;
    size_t rows;
    size_t cells;
    size_t cell_width;
};

class FMSketch
{
public:
    FMSketch(size_t cells, Hash &hash);
    void insert_element(const string &elem);
    size_t query_num_distinct() const;
private:
    static const float_t PHI = 0.77351;
    Hashtable hashtable;
    Hash hash;
    size_t cells;
    size_t tail(size_t hashval) const;
};

class CountSketch
{
public:
    CountSketch(size_t cell_width, size_t cells, size_t rows,
                const Hash &hash1, const Hash &hash2);
    void insert_element(const string &elem, size_t delta);
    size_t query_element(const string &elem) const;
private:
    Hashtable hashtable;
    Hash hash1, hash2;
    size_t cell_width;
    size_t cells;
    size_t rows;
    size_t find_median(nvec &vec) const;
};

#endif //STREAMINGLIB_SKETCH_H
