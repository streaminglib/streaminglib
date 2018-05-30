//
// Created by jin on 2018/5/7.
//

#include <cstdlib>
#include <string>
#include <vector>

#include "hash.h"

using std::string;

Murmur3Hash::
Murmur3Hash(nvec seeds) {
    this->seeds = seeds;  // 代表多个不同seed的同类哈希函数
}

nvec Murmur3Hash::
operator()(const string &s) const {
    const void *key = s.c_str();
    const int len = s.size();
    char out[128 / 8];  // 返回128位整数，使用char[16]数组保存
    nvec results; // 保存哈希值，对每一个seed生成一个哈希值

    for (auto seed: seeds) {
        MurmurHash3_x64_128(key, len, seed, out);
        results.push_back(*((size_t *) out));  // 截取out数组前几位强制转换为size_t
    }
    return results;
}