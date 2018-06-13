#include <iostream>
#include "bloom_filter.h"
#include "sketch.h"
#include "hash.h"
using namespace std;

void test_bloom_filter_mi() {
    vector<size_t> seeds = {1, 2, 3, 4};
    Murmur3Hash murmur(seeds);
    
    size_t cell_width = 8;
    size_t cells = 256;
    SpectralBloomFilter_MI filter(cell_width, cells, murmur);
    
    vector<string> input_list = { "california", "moscow", "washington", "hakane" };
    vector<string> query_list = { "california", "beijing", "moscow", "washington" };
    
    for (auto input: input_list)
        filter.insert_element(input);

    for (auto query: query_list)
        if (filter.query_element(query))
            cout << query << " in filter" << endl;
        else
            cout << query << " not in filter" << endl;
}

void test_bloom_filter_rm() {
    vector<size_t> l1_seeds = {1, 2, 3, 4, 5};
    vector<size_t> l2_seeds = {4, 5, 6, 7};
    Murmur3Hash l1_murmur(l1_seeds);
    Murmur3Hash l2_murmur(l2_seeds);
    
    size_t l1_cell_width = 4;
    size_t l1_cells = 256;
    size_t l2_cell_width = 8;
    size_t l2_cells = 128;
    SpectralBloomFilter_RM filter(l1_cell_width, l2_cell_width, l1_cells, l2_cells, l1_murmur, l2_murmur);
    
    vector<string> input_list = { "california", "moscow", "washington", "hakane" };
    vector<string> query_list = { "california", "beijing", "moscow", "washington" };
    
    for (auto input: input_list)
        filter.insert_element(input);

    for (auto query: query_list)
        if (filter.query_element(query))
            cout << query << " in filter" << endl;
        else
            cout << query << " not in filter" << endl;
}

void test_cm_sketch() {
    vector<size_t> seeds = {1, 2, 3, 4};
    Murmur3Hash murmur(seeds);
    
    size_t rows = 4;
    size_t cell_width = 8;
    size_t cells = 16;
    CMSketch filter(rows, cell_width, cells, murmur);
    
    vector<string> input_list = { "california", "moscow", "washington", "hakane" };
    vector<string> query_list = { "california", "beijing", "moscow", "washington" };
    
    for (auto input: input_list)
        filter.insert_element(input);

    for (auto query: query_list)
        if (filter.query_element(query))
            cout << query << " in filter" << endl;
        else
            cout << query << " not in filter" << endl;
}

void test_count_sketch(){
    size_t cell_width = 8;
    size_t cells = 128;
    size_t rows = 4;

    vector<size_t> seeds1 = { 3, 4, 5, 6 };
    vector<size_t> seeds2 = { 5, 6, 7, 8 };
    
    Murmur3Hash murmur1(seeds1);
    Murmur3Hash murmur2(seeds2);
    BinaryHash binary(murmur2);
    
    CountSketch sketch(cell_width, cells, rows, murmur1, binary);
    
    vector<string> input_list = { "california", "moscow", "washington", "hakane" };
    vector<size_t> delta = { 1, 2, 3, 1 };
    for (size_t i = 0; i < 4; i++)
        sketch.insert_element(input_list[i], delta[i]);
        
    vector<string> query_list = { "california", "beijing", "moscow", "washington" };
    for (auto query: query_list){
        cout << sketch.query_element(query) << ' ' << query;
        cout << " in sketch" << endl;
    }
}

void test_fm_sketch() {
    vector<size_t> seeds = { 3 };
    Murmur3Hash murmur(seeds);
    
    size_t cells = 128;
    FMSketch sketch(cells, murmur);
    
    vector<string> input_list = { "california", "moscow", "washington", "hakane" };
    vector<size_t> delta = { 1, 2, 3, 1 };
    
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < delta[i]; j++)
            sketch.insert_element(input_list[i]);
    
    cout << sketch.query_num_distinct() << " distinct items in sketch" << endl;
}

int main(int argc, char *argv[]) {
    test_fm_sketch();
    return 0;
}