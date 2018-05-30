//
// Created by jin on 2018/5/7.
//
#include <algorithm>
#include <vector>
#include <cstdio>
#include <numeric>
#include "hashtable.h"

// cell_width 目前只支持 0-32
Hashtable::Hashtable(size_t cell_width, size_t cells, size_t rows) {
    if (cell_width == 0 || cell_width > 32) {
        fprintf(stderr, "cell width must be between 1 and 32\n");
        fprintf(stderr, "cell width is set to 1\n");
        cell_width = 1;
    }

    this->cell_width = cell_width;
    this->cells = cells;
    this->rows = rows;

    table.resize(rows);
    for (auto row: table){
        row.resize(cells);
        for (auto cell: row)
            cell.resize(cell_width);
    }
}

size_t Hashtable::inner_product(const Hashtable &a, size_t rowa, const Hashtable &b, size_t rowb) {
    size_t sum = 0, len = a.table[rowa].size();
    for (size_t i = 0; i < len; i++) {
        size_t aval = Hashtable::cell_to_sizet(a.table[rowa][i]);
        size_t bval = Hashtable::cell_to_sizet(b.table[rowb][i]);
        sum += aval * bval;
    }
    return sum;
}

size_t Hashtable::get(size_t idx, int row=0) const {
    auto &target_cell = table[row][idx];
    return cell_to_sizet(target_cell);
}

void Hashtable::get(const nvec &indices, nvec &out, int row=0) const {
    if (!out.empty()) out.clear();
    for (auto i: indices)
        out.push_back(get(i, row));
}

const vector<bvec> &Hashtable::get_row(size_t row) const {
    return table[row];
}

void Hashtable::set(size_t idx, int row=0) {
    table[row][idx][0] = true;
}

void Hashtable::set(const nvec &indices, int row=0) {
    if (row < 0) {
        for (size_t i = 0; i < rows; i++)
            table[i][indices[i]][0] = true;
        return;
    }
    auto &target_row = table[row];
    for (auto i: indices)
        target_row[i][0] = true;
    return;
}

void Hashtable::reset(size_t idx, int row=0) {
    table[row][idx][0] = false;
}

void Hashtable::reset(const nvec &indices, int row=0) {
    if (row < 0) {
        for (size_t i = 0; i < rows; i++)
            table[i][indices[i]][0] = false;
        return;
    }
    auto &target_row = table[row];
    for (auto i: indices)
        target_row[i][0] = false;
    return;
}

// 按位实现加法，需检测
void Hashtable::inc(size_t idx, size_t delta, int row=0) {
    auto &target_cell = table[row][idx % cells];
    size_t cell_val = cell_to_sizet(target_cell);
    size_t carry = cell_val & delta;
    size_t result = cell_val ^ delta;
    while (carry)
    {
        carry = result & (carry << 1);
        result ^= (carry << 1);
    }
    sizet_to_cell(result, target_cell);
}

// 按位实现加法(矢量版），需检测
void Hashtable::inc(const nvec &indices, size_t delta, int row=0) {
    if (row < 0)
        for (size_t i = 0; i < rows; i++)
            inc(indices[i], delta, i);
    else
        for (auto i: indices)
            inc(i, delta, row);
}

void Hashtable::dec(size_t idx, size_t delta, int row=0) {
    delta = ~delta + 1;
    inc(idx, delta, row);
}

void Hashtable::dec(const nvec &indices, size_t delta, int row=0) {
    if (row < 0)
        for (size_t i = 0; i < rows; i++)
            dec(indices[i], delta, i);
    else
        for (auto i: indices)
            dec(i, delta, row);
}

void Hashtable::idxmin(nvec &idxmin, const nvec &mask, int row=0) const {
    if (row < 0) {
        bvec foo(cell_width, false);
        auto valmin = &foo;
        if (!idxmin.empty()) idxmin.clear();

        for (size_t i = 0; i < rows; i++) {
            auto cell = table[i][mask[i]];
            if (bitwise_compare(cell, *valmin) > 0) continue;
            valmin = &cell;
        }

        for (size_t i = 0; i < row; i++) {
            if (!bitwise_compare(table[i][mask[i]], *valmin))
                idxmin.push_back(i);
        }
        return;
    }

    auto &target_row = table[row];
    bvec foo(cell_width, false);
    auto valmin = &foo;

    if (!idxmin.empty()) idxmin.clear();

    for (auto i: mask) {
        auto cell = target_row[i];
        if (bitwise_compare(cell, *valmin) > 0) continue;
        valmin = &cell;
    }
    for (auto i: mask)
        if (!bitwise_compare(target_row[i], *valmin))
            idxmin.push_back(i);
}

size_t Hashtable::minimum(const nvec &mask, int row=0) const {
    if (row < 0) {
        nvec idxmin;
        this->idxmin(idxmin, mask, -1);
        return get(mask[idxmin[0]], idxmin[0]);
    }
    nvec idxmin;
    this->idxmin(idxmin, mask, row);
    return get(idxmin[0], row);
}

size_t Hashtable::cell_to_sizet(const bvec &cell) const {
    size_t i = (size_t)std::accumulate(cell.rbegin(), cell.rend(), 0,
                                  [](int x, int y) { return (x<<1) + y; });
    return i;
}

void Hashtable::sizet_to_cell(size_t val, bvec &cell) {
    size_t idx = cell_width;
    while (val && idx) {
        cell[--idx] = (val & 1);
        val >>= 1;
    }
}

int Hashtable::bitwise_compare(const bvec &a, const bvec &b) const {
    for (int idx = cell_width-1; idx >= 0; idx--) {
        bool aa = a[idx], bb = b[idx];
        if (aa == bb) continue;
        else if (aa && !bb) return 1;
        else if (!aa && bb) return -1;
    }
    return 0;
}








