#ifndef STREAMINGLIB_COLD_FILTER_H
#define STREAMINGLIB_COLD_FILTER_H

#include "hash.h"
#include "hashtable.h"


class ColdFilter
{
public:
    ColdFilter(size_t l1_cell_width, size_t l2_cell_width,
               size_t l1_cells, size_t l2_cells,
               size_t l1_thres, size_t l2_thres,
               Hash &l1_hash, Hash &l2_hash);
    bool insert_and_report(const string &elem);
    bool insert_and_report(const string &elem, size_t num, size_t &repo_freq);
    bvec insert_and_report(const vector<string> &elem);
    bvec insert_and_report(const vector<string> &elem, nvec num, nvec &repo_freq);
private:
    Hashtable L1_table, L2_table;
    Hash &L1_hash, &L2_hash;
    size_t l1_cell_width, l2_cell_width;
    size_t l1_cells, l2_cells;
    size_t l1_thres, l2_thres;
};

class Bucket;

class ColdFilter_Aggregate
{
public:
    ColdFilter_Aggregate(ColdFilter &filter, size_t buckets,
                         size_t bucket_cells, Hash &bucket_hash);
    bool insert_and_report(const string &elem, size_t id, size_t &repo_freq);
    bvec insert_and_report(const vector<string> &elem, nvec id, nvec &repo_freq);
private:
    size_t bucket_cells;
    ColdFilter &filter;
    vector<Bucket> buckets;
    Hash &bucket_hash;
};


class Bucket
{
friend ColdFilter_Aggregate;
public:
    bool insert(size_t id, size_t &repo_id, size_t &repo_freq);
    void resize(size_t cells);
private:
    size_t cells;
    nvec id;
    nvec freq;
};

#endif //STREAMINGLIB_COLD_FILTER_H