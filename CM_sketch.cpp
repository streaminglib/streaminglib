//
// Created by jin on 2018/3/26.
// a naive cm sketch implementation
//

#include <cmath>
#include <iostream>
#include <cstring>
#include "sketch_base.hpp"

template <typename T>
CMSketch<T>::CMSketch(double eps = default_eps,
                      double delta = default_delta,
                      int width = 0, int depth = 0): hash_func(hash)
{
    if (eps <= 0 || delta <= 0)
    {
        std::cerr << "ArgumentError: eps <= 0 or delta <= 0\n"
                  << "Setting eps and delta to default value\n";
        eps = default_eps;
        delta = default_delta;
    }

    hash_funcs = new hash_ptr[depth];
    for (int j = 0; j < depth; ++j)
        hash_funcs[j] = NULL;

    if (width <= 0 || depth <= 0)
    {
        depth = (int) ceil(M_E/eps);
        width = (int) ceil(-log(delta));
    }
    table_ptr = new int*[depth];
    for (int i = 0; i < depth; i++)
    {
        table_ptr[i] = new int[width];
        memset(table_ptr[i], 0, width * sizeof(int));
    }
}

template <typename T>
CMSketch<T>::~CMSketch() {
    for(int i = 0; i < depth; i++)
    {
        if (table_ptr[i] != NULL)
            delete[](table_ptr[i]);
        if (hash_funcs[i] != NULL)
            delete(hash_funcs[i]);
    }
    delete[](hash_funcs);
    delete[](table_ptr);
}

int CMSketch::get_depth() const
{
    return depth;
}

int CMSketch::get_width() const
{
    return width;
}

void CMSketch::set_hash(hash_ptr *hash_ptrs)
{
    hash_funcs = hash_ptrs;
}

void CMSketch::update(T &item, int num)
{
    int hash_val = 0;
    for (int i = 0; i < depth; ++i) {
        hash_val = hash_funcs[i](item);
        table_ptr[i][hash_val] |= 1;
    }
}

int CMSketch::point_query(T &item) const
{
    int hash_val = 0;
    auto ans = INT32_MAX;
    for (int i = 0; i < depth; ++i) {
        hash_val = hash_funcs[i](item);
        ans = MIN(ans, hash_val);
    }
    return ans;
}

Sketch::~Sketch() {
    std::cout << "Pure virtual ~Sketch()" << endl;
}





