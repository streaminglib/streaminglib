//
// Created by jin on 2018/5/7.
//

#include <algorithm>
#include <vector>
#include <cstdio>
#include <numeric>
#include <iostream>
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
    for (size_t i = 0; i < rows; i++) {
        table[i].resize(cells);
        for (size_t j = 0; j < cells; j++)
            table[i][j].resize(cell_width);
    }
    
    lock_table.resize(rows);
    for (size_t i = 0; i < rows; i++) {
        lock_table[i].resize(cells);
        for (size_t j = 0; j < cells; j++)
            omp_init_lock(&lock_table[i][j]);
    }
    
//    omp_init_lock(&global_read_lock);
//    omp_init_lock(&global_write_lock);
}

size_t Hashtable::inner_product(Hashtable &a, size_t rowa, Hashtable &b, size_t rowb) {
    size_t sum = 0, len = a.table[rowa].size();
    for (size_t i = 0; i < len; i++) {
        omp_set_lock(&(a.lock_table[rowa][i]));
        omp_set_lock(&(b.lock_table[rowb][i]));
    }
    for (size_t i = 0; i < len; i++) {
        size_t aval = Hashtable::cell_to_sizet(a.table[rowa][i]);
        size_t bval = Hashtable::cell_to_sizet(b.table[rowb][i]);
        sum += aval * bval;
    }
    for (size_t i = 0; i < len; i++) {
        omp_unset_lock(&a.lock_table[rowa][i]);
        omp_unset_lock(&b.lock_table[rowb][i]);
    }
    return sum;
}

size_t Hashtable::get(size_t idx, int row) const {
    auto &target_cell = table[row][idx%cells];
    return cell_to_sizet(target_cell);
}

void Hashtable::get(const nvec &indices, nvec &out, int row) const {
    if (!out.empty()) out.clear();
    for (auto i: indices)
        out.push_back(get(i, row));
}

const vector<bvec> &Hashtable::get_row(size_t row) const {
    return table[row];
}

void Hashtable::set(size_t idx, int row) {
    table[row][idx][0] = true;
}

void Hashtable::set(const nvec &indices, int row) {
    if (row < 0) {
        for (size_t i = 0; i < rows; i++)
            set(i, indices[i]);
        return;
    }
    else
        for (auto i: indices) set(row, i);
        
    return;
}

void Hashtable::reset(size_t idx, int row) {
    table[row][idx][0] = false;
}

void Hashtable::reset(const nvec &indices, int row) {
    if (row < 0) {
        for (size_t i = 0; i < rows; i++)
            reset(i, indices[i]);
        return;
    }
    else
        for (auto i: indices) reset(row, i);
    return;
}

// 按位实现加法，需检测
void Hashtable::inc(size_t idx, int delta, int row) {
    auto &target_cell = table[row][idx % cells];
    omp_set_lock(&lock_table[row][idx % cells]);
    size_t cell_val = cell_to_sizet(target_cell);
    size_t result = cell_val + delta;
    sizet_to_cell(result, target_cell);
    omp_unset_lock(&lock_table[row][idx % cells]);
    std::cout << cell_val << " -> " << result << std::endl;
}

// 按位实现加法(矢量版），需检测
void Hashtable::inc(const nvec &indices, int delta, int row) {
    if (row < 0)
        for (size_t i = 0; i < rows; i++)
            inc(indices[i], delta, i);
    else
        for (auto i: indices)
            inc(i, delta, row);
}

void Hashtable::inc(const nvec &indices, const vector<int> &delta, int row) {
    if (row < 0)
        for (size_t i = 0; i < rows; i++)
            inc(indices[i], delta[i], i);
    else {
        for (size_t i = 0; i < rows; i++)
            inc(indices[i], delta[i], row);
    }
}

void Hashtable::dec(size_t idx, size_t delta, int row) {
    delta = ~delta + 1;
    inc(idx, delta, row);
}

void Hashtable::dec(const nvec &indices, size_t delta, int row) {
    if (row < 0)
        for (size_t i = 0; i < rows; i++)
            dec(indices[i], delta, i);
    else
        for (auto i: indices)
            dec(i, delta, row);
}

void Hashtable::idxmin(nvec &idxmin, const nvec &mask, int row) const {
    nvec scaled_mask = mask;
    
    for (size_t i = 0; i < scaled_mask.size(); i++)
        scaled_mask[i] %= this->cells;
        
    if (row < 0) {
        size_t valmin = 0;
        if (!idxmin.empty()) idxmin.clear();

        for (size_t i = 0; i < rows; i++) {
            auto cell = table[i][scaled_mask[i]];
            if (bitwise_compare(cell, table[valmin][scaled_mask[valmin]]) > 0) continue;
            valmin = i;
        }

        for (size_t i = 0; i < rows; i++) {
            if (!bitwise_compare(table[i][scaled_mask[i]], table[valmin][scaled_mask[valmin]]))
                idxmin.push_back(i);
        }
    }
    
    else {
        auto &target_row = table[row];
        size_t valmin = scaled_mask[0];

        if (!idxmin.empty()) idxmin.clear();
            
        for (auto i: scaled_mask) {
            auto cell = target_row[i];
            if (bitwise_compare(cell, target_row[valmin]) > 0) continue;
            valmin = i;
        }
        
        for (auto i: scaled_mask) {
            if (!bitwise_compare(target_row[i], target_row[valmin]))
                idxmin.push_back(i);
        }
    }
}

size_t Hashtable::minimum(const nvec &mask, int row) const {
    nvec scaled_mask = mask;
    size_t result;
    
    for (size_t i = 0; i < scaled_mask.size(); i++)
        scaled_mask[i] %= this->cells;
        
    if (row < 0) {
        size_t valmin = 0;
        
        for (size_t i = 0; i < rows; i++) {
            auto cell = table[i][scaled_mask[i]];
            if (bitwise_compare(cell, table[valmin][scaled_mask[valmin]]) > 0) continue;
            valmin = i;
        }
        
        result = cell_to_sizet(table[valmin][scaled_mask[valmin]]);
    }

    else {
        auto &target_row = table[row];
        size_t valmin = scaled_mask[0];
    
        for (auto i: scaled_mask) {
            auto cell = target_row[i];
            if (bitwise_compare(cell, target_row[valmin]) > 0) continue;
            valmin = i;
        }
        
        result = cell_to_sizet(target_row[valmin]);
    }
    return result;
}

void Hashtable::
max(const nvec &indices, size_t n, int row) {
    if (row < 0) {
        for (size_t i = 0; i < rows; i++) {
            omp_set_lock(&lock_table[i][indices[i]]);
            size_t val = cell_to_sizet(table[i][indices[i]]);
            if (val < n)
                sizet_to_cell(n, table[i][indices[i]]);
            omp_unset_lock(&lock_table[i][indices[i]]);
        }
    }
    else {
        auto target_row = table[row];
        size_t len = indices.size();

        for (size_t i = 0; i < len; i++) {
            omp_set_lock(&lock_table[row][indices[i]]);
            size_t val = cell_to_sizet(target_row[indices[i]]);
            if (val < n)
                sizet_to_cell(n, target_row[indices[i]]);
            omp_unset_lock(&lock_table[row][indices[i]]);
        }
    }
}

void Hashtable::
assign(const nvec &indices, size_t n, int row) {
    if (row < 0) {
        for (size_t i = 0; i < rows; i++) {
            omp_set_lock(&lock_table[i][indices[i]]);
            sizet_to_cell(n, table[i][indices[i]]);
            omp_unset_lock(&lock_table[i][indices[i]]);
        }
    }
    else {
        auto target_row = table[row];
        size_t len = indices.size();
        
        for (size_t i = 0; i < len; i++) {
            omp_set_lock(&lock_table[row][indices[i]]);
            sizet_to_cell(n, target_row[indices[i]]);
            omp_unset_lock(&lock_table[row][indices[i]]);
        }
    }

}

//void Hashtable::set_global_read_lock() { omp_set_lock(&global_read_lock); }
//void Hashtable::set_global_write_lock() { omp_set_lock(&global_write_lock); }
//void Hashtable::unset_global_read_lock() { omp_unset_lock(&global_read_lock); }
//void Hashtable::unset_global_write_lock() { omp_unset_lock(&global_write_lock); }

size_t Hashtable::cell_to_sizet(const bvec &cell) {
    size_t i = (size_t)std::accumulate(cell.begin(), cell.end(), 0,
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
