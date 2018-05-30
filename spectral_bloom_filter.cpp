//
// Created by jin on 2018/5/7.
//

#include <iostream>
#include <vector>

#include "hashtable.h"
#include "hash.h"
#include "bloom_filter.h"

SpectralBloomFilter_Base::
SpectralBloomFilter_Base(
        size_t cell_width, size_t cells, Hash &hash)
        : hashtable(cell_width, cells), hash(hash) {
    this->cell_width = cell_width;
    this->cells = cells;
}

void SpectralBloomFilter_Base::
insert_element(const string &elem) {
    auto indices = hash(elem);
    hashtable.inc(indices, 1);
}

size_t SpectralBloomFilter_Base::
query_element(const string &elem) const {
    auto indices = hash(elem);
    return hashtable.minimum(indices);
}

void SpectralBloomFilter_Base::
delete_element(const string &elem) {
    auto indices = hash(elem);
    hashtable.dec(indices, 1);
}

SpectralBloomFilter_MI::
SpectralBloomFilter_MI(
        size_t cell_width, size_t cells, Hash &hash)
        : hashtable(cell_width, cells) {
    this->hash = hash;
    this->cell_width = cell_width;
    this->cells = cells;
}

void SpectralBloomFilter_MI::
insert_element(const string &elem) {
    auto indices = hash(elem);
    nvec idxmin;
    hashtable.idxmin(idxmin, indices);
    hashtable.inc(idxmin, 1);
}

size_t SpectralBloomFilter_MI::
query_element(const string &elem) const {
    nvec indices = hash(elem);
    return hashtable.minimum(indices);
}

SpectralBloomFilter_RM::
SpectralBloomFilter_RM(
        size_t l1_cell_width, size_t l2_cell_width,
        size_t l1_cells, size_t l2_cells,
        Hash &l1_hash, Hash &l2_hash)
        : L1_SBF(l1_cell_width, l1_cells, l1_hash),
          L2_SBF(l2_cell_width, l2_cells, l2_hash){}

void SpectralBloomFilter_RM::
insert_element(const string &elem) {
    nvec l1_idxmin;
    nvec l1_indices = L1_SBF.hash(elem);
    L1_SBF.hashtable.inc(l1_indices, 1);
    L1_SBF.hashtable.idxmin(l1_idxmin, l1_indices);
    if (l1_idxmin.size() > 1) return;

    nvec l2_idxmin;
    nvec l2_indices = L2_SBF.hash(elem);
    size_t l2_minimum = L2_SBF.hashtable.minimum(l2_indices);
    if (l2_minimum > 0)
        L2_SBF.hashtable.inc(l2_indices, 1);
    else {
        size_t delta = L1_SBF.hashtable.get(l1_idxmin[0]);
        L2_SBF.hashtable.inc(l2_indices, delta);
    }
}

size_t SpectralBloomFilter_RM::
query_element(const string &elem) const {
    size_t l1_result = L1_SBF.query_element(elem);
    nvec l1_idxmin;
    L1_SBF.hashtable.idxmin(l1_idxmin, L1_SBF.hash(elem));
    bool recur_minimum = l1_idxmin.size() > 1;
    if (recur_minimum) return l1_result;
    size_t l2_result = L2_SBF.query_element(elem);
    if (l2_result > 0) return l2_result;
    else return l1_result;
}

