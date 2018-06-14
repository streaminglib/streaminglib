//
// Created by jin on 2018/5/30.
//

#include "cold_filter.h"

ColdFilter::ColdFilter(size_t l1_cell_width, size_t l2_cell_width,
                    size_t l1_cells, size_t l2_cells,
                    size_t l1_thres, size_t l2_thres,
                    Hash &l1_hash, Hash &l2_hash): 
                        L1_table(l1_cell_width, l1_cells),
                        L2_table(l2_cell_width, l2_cells),
                        L1_hash(l1_hash), L2_hash(l2_hash) {
    this->l1_cell_width = l1_cell_width;
    this->l2_cell_width = l2_cell_width;
    this->l1_cells = l1_cells;
    this->l2_cells = l2_cells;
    this->l1_thres = l1_thres;
    this->l2_thres = l2_thres;
}

bool ColdFilter::
insert_and_report(const string &elem) {
    nvec idxmin;

    nvec l1_indices = this->L1_hash(elem);
    size_t l1_min = this->L1_table.minimum(l1_indices);
    if (l1_min < this->l1_thres) {
        this->L1_table.idxmin(idxmin, l1_indices);
        this->L1_table.inc(idxmin, 1);
        return false;
    }

    nvec l2_indices = this->L2_hash(elem);
    size_t l2_min = this->L2_table.minimum(l2_indices);
    if (l2_min >= this->l2_thres) return true;
    this->L2_table.idxmin(idxmin, l2_indices);
    this->L2_table.inc(idxmin, 1);
    return false;
}

bool ColdFilter::
insert_and_report(const string &elem, size_t num, size_t &repo_freq) {
    nvec idxmin;

    nvec l1_indices = this->L1_hash(elem);
    size_t l1_min = this->L1_table.minimum(l1_indices);
    if (l1_min+num <= this->l1_thres) {
        this->L1_table.max(l1_indices, l1_min+num);
        repo_freq = 0;
        return false;
    }
    this->L1_table.assign(l1_indices, this->l1_thres);
    num = num - (this->l1_thres - l1_min);

    nvec l2_indices = this->L2_hash(elem);
    size_t l2_min = this->L2_table.minimum(l2_indices);
    if (l2_min + num <= this->l2_thres) {
        this->L2_table.max(l2_indices, l2_min+num);
        repo_freq = 0;
        return false;
    }
    this->L2_table.assign(l2_indices, this->l2_thres);
    num = num - (this->l2_thres - l2_min);
    repo_freq = num;
    return true;
}

bvec ColdFilter::
insert_and_report(const vector<string> &elem) {
    bvec results;
    size_t len = elem.size();
    
    results.resize(len);
    
#pragma omp parallel for
    for (size_t i = 0; i < len; i++)
        results[i] = insert_and_report(elem[i]);
    
    return results;
}

bvec ColdFilter::
insert_and_report(const vector<string> &elem, nvec num, nvec &repo_freq) {
    bvec results;
    size_t len = elem.size();
    
    results.resize(len);
    if (!repo_freq.empty()) 
        repo_freq.clear();
    repo_freq.resize(len);
    
#pragma omp parallel for
    for (size_t i = 0; i < len; i++)
        results[i] = insert_and_report(elem[i], num[i], repo_freq[i]);
    
    return results;
}

ColdFilter_Aggregate::
ColdFilter_Aggregate(ColdFilter &filter, size_t buckets,
                     size_t bucket_cells, Hash &bucket_hash)
                     : filter(filter), bucket_hash(bucket_hash){
    this->buckets.resize(buckets);
    for (auto b: this->buckets)
        b.resize(bucket_cells);
}

bool ColdFilter_Aggregate::
insert_and_report(const string &elem, size_t id, size_t &repo_freq) {
    size_t repo_id, bucket_repo_freq;
    bool overflow = false;
    size_t bucket_idx = this->bucket_hash(elem)[0];
    overflow = this->buckets[bucket_idx].insert(id, repo_id, bucket_repo_freq);
    if (!overflow) {
        repo_freq = 0;
        return false;
    }
    overflow = this->filter.insert_and_report(elem, bucket_repo_freq, repo_freq);
    return overflow;
}

bvec ColdFilter_Aggregate::
insert_and_report(const vector<string> &elem, nvec id, nvec &repo_freq) {
    bvec results;
    size_t len = elem.size();
    
    results.resize(len);
    if (!repo_freq.empty())
        repo_freq.clear();
    repo_freq.resize(len);
    
#pragma omp parallel for
    for (size_t i = 0; i < len; i++)
        results[i] = insert_and_report(elem[i], id[i], repo_freq[i]);
    
    return results;
}

void Bucket::
resize(size_t cells) {
    this->cells = cells;
    this->id.resize(cells);
    this->freq.resize(cells);
}

bool Bucket::
insert(size_t id, size_t &repo_id, size_t &repo_freq) {
    ssize_t vacancy = -1;
    for (size_t i = 0; i < this->cells; i++)
        if (this->id[i] == 0 && vacancy == -1)
            vacancy = i;
        else if (this->id[i] == id) {
            this->freq[i] ++;
            return false;
        }
    if (vacancy == -1) {
        repo_id = this->id[0];
        repo_freq = this->freq[0];
        this->id[0] = id;
        this->freq[0] = 1;
        return true;
    }
    else {
        this->id[vacancy] = id;
        this->freq[vacancy] = 1;
        return false;
    }
}
