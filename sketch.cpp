//
// Created by jin on 2018/5/8.
//

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "hashtable.h"
#include "hash.h"
#include "sketch.h"

CMSketch::
CMSketch(size_t rows, size_t cell_width, size_t cells, Hash &hash)
        : hashtable(cell_width, cells, rows), hash(hash) {
    this->rows = rows;
    this->cells = cells;
    this->cell_width = cell_width;
}

void CMSketch::
insert_element(const string &elem, size_t delta) {
    nvec indices = hash(elem);
    hashtable.inc(indices, delta, -1);
}

size_t CMSketch::
query_element(const string &elem) const {
    nvec indices = hash(elem);
    size_t result = hashtable.minimum(indices, -1);
    return result;
}

size_t CMSketch::
query_inner_product(const CMSketch &cms) const {
    size_t result = Hashtable::inner_product(hashtable, 0, cms.hashtable, 0);
    for (size_t i = 1; i < rows; i++){
        size_t inner_prod = Hashtable::inner_product(hashtable, i, cms.hashtable, i);
        if (inner_prod < result)
            result = inner_prod;
    }
    return result;
}

void CMSketch::
insert_element(const vector<string> &elem, nvec delta) {
    size_t n = elem.size();
    for (size_t i = 0; i < n; i++)
        insert_element(elem[i], delta[i]);
}

nvec CMSketch::
query_element(const vector<string> &elem) const {
    nvec results;
    size_t n = elem.size();
    size_t result;

    for (size_t i = 0; i < n; i++) {
        result = query_element(elem[i]);
        results.push_back(result);
    }
    
    return results;
}

// TODO: range query

FMSketch::FMSketch(size_t cells, Hash &hash)
        : hashtable(1, cells, 1), hash(hash) {
    this->cells = cells;
}

void FMSketch::insert_element(const string &elem) {
    size_t index = tail(hash(elem)[0]);
    hashtable.set(index);
}

size_t FMSketch::tail(size_t hashval) const {
    size_t len = sizeof(size_t);
    for (size_t i = 0; i < len; i++)
        if (hashval & (1 << 0)) return i;
    return len;
}

size_t FMSketch::
query_num_distinct() const {
    auto row = hashtable.get_row(0);
    for (size_t i = 0; i < cells; i++) {
        if (row[i][0]) continue;
        else return size_t((float(1 << i)) / PHI);
    }
    return size_t((float(1 << (cells+1))) / PHI);
}

void FMSketch::
insert_element(const vector<string> &elem) {
    size_t n = elem.size();
    for (size_t i = 0; i < n; i++)
        insert_element(elem[i]);
}

CountSketch::
CountSketch(size_t cell_width, size_t cells, size_t rows,
            Hash &hash1, Hash &hash2)
        : hashtable(cell_width, cells, rows), hash1(hash1), hash2(hash2) {
    this->cell_width = cell_width;
    this->cells = cells;
    this->rows = rows;
}

void CountSketch::
insert_element(const string &elem, size_t delta) {
    auto indices = hash1(elem);
    auto inc_dec = hash2(elem);
    for (size_t i = 0; i < rows; i++) {
        if (inc_dec[i] > 0)
            hashtable.inc(indices[i], delta, i);
        else
            hashtable.dec(indices[i], delta, i);
    }
}

size_t CountSketch::
query_element(const string &elem) const {
    auto indices = hash1(elem);
    auto inc_dec = hash2(elem);
    nvec result;

    for (size_t i = 0; i < rows; i++) {
        size_t temp = hashtable.get(indices[i], i);
        result.push_back(inc_dec[i] * temp);
    }
    return find_median(result);
}

void CountSketch::
insert_element(const vector<string> &elem, nvec delta) {
    size_t n = elem.size();
    for (size_t i = 0; i < n; i++)
        insert_element(elem[i], delta[i]);
}

nvec CountSketch::
query_element(const vector<string> &elem) const {
    nvec results;
    size_t n = elem.size();
    size_t result;
    
    for (size_t i = 0; i < n; i++) {
        result = query_element(elem[i]);
        results.push_back(result);
    }
    
    return results;
}

size_t CountSketch::
find_median(nvec &vec) const {
    size_t mid = vec.size() / 2;
    std::nth_element(vec.begin(), vec.begin()+mid, vec.end());
    return vec[mid];
}








