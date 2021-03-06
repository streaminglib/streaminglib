#ifndef STREAMINGLIB_AMS_SKETCH_H
#define STREAMINGLIB_AMS_SKETCH_H

#include <string>
#include <cstdlib>
#include <functional>
#include <algorithm>

#include "hash.h"
#include "hashtable.h"
using namespace std;

class ams_sketch{
public:
    ams_sketch(size_t c, size_t c_w, vector<int> s);
    void insert_element(const string & ele, int freq = 1);
    void insert_element(const vector<string> &ele, vector<int> freq);
    int get_estimateF2();
    int obj_count(const string & ele);
    vector<int> obj_count(const vector<string> &ele);
private:
    Hashtable hash_table;
    std::vector<int> seeds;
    size_t cells;
    size_t cell_width;

    int update_sketch(int mult, int idx, int hash, int freq);
    void get_estimate(int hash, int mult, int idx, std::vector<int> & estimates);
    void mult_by_position(Hashtable & s1, Hashtable & s2, vector<int> & estimates);
    int get_final_estimates(std::vector<int> & estimates);
};

#endif // STREAMINGLIB_AMS_SKETCH_H