//
// Created by jin on 2018/5/7.
//

#ifndef STREAMINGLIB_HASHTABLE_H
#define STREAMINGLIB_HASHTABLE_H

#include <vector>
#include <omp.h>

typedef std::vector<size_t> nvec;
typedef std::vector<bool> bvec;
using std::vector;

class Hashtable
{
public:
    // cell_width 为每一个 counter 包含的 bit 数，cells 为 counter 数
    Hashtable(size_t cell_width, size_t cells, size_t rows=1);
    static size_t inner_product(const Hashtable &a, size_t rowa, const Hashtable &b, size_t rowb);
    // 获取特定位置的 counter get([1, 2, 3], out, row=0): out <- [t[0][1], t[0][2], t[0][3]]
    const vector<bvec> &get_row(size_t row) const;
    size_t get(size_t idx, int row=0) const;
    void get(const nvec &indices, nvec &out, int row=0) const;
    void set(size_t idx, int row=0);
    void set(const nvec &indices, int row=0);
    void reset(size_t idx, int row=0);
    void reset(const nvec &indices, int row=0);
    // 给特定位置的 counter
    void inc(size_t idx, int delta, int row=0);
    void inc(const nvec &indices, int delta, int row=0);
    void inc(const nvec &indices, const vector<int> &deltas, int row=0);
    void dec(size_t idx, size_t delta, int row=0);
    void dec(const nvec &indices, size_t delta, int row=0);
    // 取得特定行特定部分所有最小 counter 的索引
    // idxmin(output, [1, 3, 5], row=0): output = [1, 5] if t[0] = [5, 2, 3, 4, 4, 2]
    void idxmin(nvec &idxmin, const nvec &mask, int row=0) const;
    // 取得特定行特定部分最小 counter 的值
    size_t minimum(const nvec &mask, int row=0) const;
    void max(const nvec &indices, size_t n, int row=0);
    void assign(const nvec &indices, size_t n, int row=0);

private:
    size_t cell_width;
    size_t cells;
    size_t rows;
    // 基本数据结构，最深的 vector 代表一个 counter
    // 故实际上是 2D counter 表。
    // bvec: vector<bool>
    std::vector<std::vector<bvec>> table;

    // 辅助函数，将 counter 转换为 size_t 的值，int.frombytes
    static size_t cell_to_sizet(const bvec &cell);
    // 辅助函数，将 size_t 的值存到 counter(cell) 中
    void sizet_to_cell(size_t val, bvec &cell);
    // 即字典序比较，与 comp(cell_to_sizet(a), cell_to_sizet(b)) 等价
    // 返回 -1， 0， 1: a < b, a = b, a > b;
    int bitwise_compare(const bvec &a, const bvec &b) const;
};

#endif //STREAMINGLIB_HASHTABLE_H
