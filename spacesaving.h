#ifndef STREAMING_LIB_SPACEVING_H
#define STREAMING_LIB_SPACEVING_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "hash.h"

struct node{
    int value;
    int freq;
    int over_est;
    node(){
        value = -1;
        freq = -1;
        over_est = 0;
    }
    node(int v, int f){
        value = v;
        freq = f;
        over_est = 0;
    }
    friend operator<(const node& obj1, const node& obj2){
        return obj1.freq < obj2.freq;// well...i did not use it!
    }
};

class space_saving{
public:
    std::vector<node> heap; // should be an array
    int size;
    int used;
    int n;
    space_saving();
    space_saving(int s);
    void insert(int val, int freq = 1);
    void query_top(std::vector<node> & res);
private:
    void heapify(int idx);
    void make_heap();
};

#endif // STREAMING_LIB_SPACEVING_H
