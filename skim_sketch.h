#ifndef STREAMINGLIB_SKIM_SKETCH_H
#define STREAMINGLIB_SKIM_SKETCH_H

#include <set>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdlib.h>
#include <iostream>

#include "hash.h"
#include "hashtable.h"
using namespace std;

const size_t s1 = 64;
const size_t s2 = 256;

// my simulation of hashtable, this works!
class Hash_table{
public:
    std::vector<std::vector<int> > table;
    Hash_table(int i, int j, int k){
        table.resize(k);
        for(int l = 0; l < k; l += 1){
            table[l].resize(j);
        }
    }
    int get(int i, int j){
        return table[j][i];
    }
    void inc(int i, int del, int j){
        table[j][i] += del;
    }
};

class skim_sketch{
public:
    long long join_size;
    skim_sketch();
    skim_sketch(std::vector<int> & f1, std::vector<int> & f2, int domain_size);
private:
    Hash_table h1;
    Hash_table h2;
    std::vector<int> e1;
    std::vector<int> e2;
    int flow_length1;
    int flow_length2;
    int est_skim_join_size();
    void skim_dense(Hash_table & h, int & flow_length, std::vector<int> & e);
    int est_sub_join_size(std::vector<int> & e, Hash_table & h);
};

int hash_template(int seed, int n);
void get_hash_table(std::vector<int> & f, Hash_table & h);
int inner_product(vector<int> &p, vector<int> &q, int length);
int median(vector<int> p, int length);
int epsilon(int a, int b);

#endif // STREAMINGLIB_SKIM_SKETCH_H
