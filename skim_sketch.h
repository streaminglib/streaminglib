#ifndef STREAMINGLIB_SKIM_SKETCH_H
#define STREAMINGLIB_SKIM_SKETCH_H

#include <set>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdlib.h>

#include "hash.h"
#include "hashtable.h"
using namespace std;

const size_t s1 = 64;
const size_t s2 = 128;

class skim_sketch{
public:
    long long join_size;
    skim_sketch();
    skim_sketch(std::vector<int> f1, std::vector<int> f2, int domain_size);
private:
    Hashtable h1;
    Hashtable h2;
    std::vector<int> e1;
    std::vector<int> e2;
    int flow_length1;
    int flow_length2;
    int est_skim_join_size();
    void skim_dense(Hashtable & h, int & flow_length, std::vector<int> & e);
    int est_sub_join_size(std::vector<int> & e, Hashtable & h);
};

int hash_template(int seed, int n);
void get_hash_table(std::vector<int> & f, Hashtable & h);
int inner_product(vector<int> &p, vector<int> &q, int length);
int median(vector<int> p, int length);

#endif // STREAMINGLIB_SKIM_SKETCH_H