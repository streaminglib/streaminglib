#include <iostream>
#include "ams_sketch.h"
#include "spacesaving.h"
#include "skim_sketch.h"
using namespace std;

void test_ams_sketch(){
    // some seeds for hash functions, specified by user
    std::vector<size_t> seeds = {2, 3, 5, 7};
    // 4 hash functions, each has 256 cells
    ams_sketch as(4, 256, seeds);

    std::vector<char* > insert_ele = {"california", "moscow", "washington", "hakane"};
    std::vector<char* > query_list = {"california", "moscow", "washington", "hakane"};

    for(auto input: insert_ele){
        // the second arg specifies the frequency, default freq = 1
        as.insert_element(input, 1);
    }
    // obj count test
    for(auto query: query_list){
        // how many time does this obj appeared?
        if(as.obj_count(query)){
            cout << query << " in filter" << endl;
        }else{
            cout << query << " not in filter" << endl;
        }
    }
    // F2 test
    as.insert_element(insert_ele[0]);
    // what is its norm(squared) of its frequency vector?
    cout << "the estimation of F2 is " << as.get_estimateF2() << endl;
    return ;
}

void test_skim_sketch(){
    // two frequency vectors given by user
    std::vector<int> f1 = {50, 50, 10, 5};
    std::vector<int> f2 = {50, 5, 10, 50};
    // the third arg specifies domain size(length of frequency vector)
    skim_sketch ss(f1, f2, 4);

    // "join size" in this paper actually refers to
    // the inner product of these two frequency vectors
    // we tested it locally, and reached about 10-20% error rate(on uniformed distributed data)
    // compared to <7% in the original paper
    cout << "the estimation of join size" << ss.join_size << endl;
}


int main(){
    test_skim_sketch();
    test_ams_sketch();
    return 0;
}
