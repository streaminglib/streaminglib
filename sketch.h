 //
// Created by jin on 2018/5/8.
//

#ifndef STREAMINGLIB_SKETCH_H
#define STREAMINGLIB_SKETCH_H

#include "hash.h"
#include "hashtable.h"
#include "cold_filter.h"
#include "reader.h"

class CMSketch
{
public:
    CMSketch(size_t rows, size_t cell_width, size_t cells, Hash &hash);
    CMSketch(size_t rows, size_t cell_width, size_t cells, Hash &hash, ColdFilter &cf);
    void insert_element(const string &elem, size_t delta=1);
    size_t query_element(const string &elem) const;
    size_t query_inner_product(CMSketch &cms);
    void insert_element(const vector<string> &elem, nvec delta);
    nvec query_element(const vector<string> &elem) const;
    void insert_element(const string &id_file, const string &freq_file);
    nvec query_element_from_file(const string &filename) const;
private:
    Hashtable hashtable;
    Hash &hash;
    ColdFilter *cf_ptr;
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
    void insert_element(const vector<string> &elem);
private:
    static constexpr float PHI = 0.77351;
    Hashtable hashtable;
    Hash &hash;
    size_t cells;
    size_t tail(size_t hashval) const;
};

class CountSketch
{
public:
    CountSketch(size_t cell_width, size_t cells, size_t rows,
                Hash &hash1, Hash &hash2);
    void insert_element(const string &elem, size_t delta);
    size_t query_element(const string &elem) const;
    void insert_element(const vector<string> &elem, nvec delta);
    nvec query_element(const vector<string> &elem) const;
private:
    Hashtable hashtable;
    Hash &hash1, &hash2;
    size_t cell_width;
    size_t cells;
    size_t rows;
    size_t find_median(nvec &vec) const;
};

#endif //STREAMINGLIB_SKETCH_H
