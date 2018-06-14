import string
import random
import time

import numpy as np

import bloom_filter as bf


def id_gen(size=6, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))

def rand_id_list(size=6, len=1000):
    return [ id_gen(size) for i in range(len)]
    
def skewed_freq(size=1000):
    return 20 * np.random.zipf(2, size=size)
    
if __name__ == '__main__':
    test_size = 10000
    
    rows = 16
    cell_width = 12
    cells = 100000
    
    l1_cells = 256
    l1_cell_width = 2
    l1_thres = 3
    l2_cells = 64
    l2_cell_width = 6
    l2_thres = 63
    l1_seeds = range(3)
    l2_seeds = range(3)
    l1_hash = bf.Murmur3Hash(l1_seeds)
    l2_hash = bf.Murmur3Hash(l2_seeds)
    
    id_list = rand_id_list(size=test_size)
    freqs = skewed_freq(test_size)
    
    cf = bf.ColdFilter(l1_cell_width, l2_cell_width, l1_cells, l2_cells, l1_thres, l2_thres, l1_hash, l2_hash)

    seeds = range(rows)
    murmur = bf.Murmur3Hash(seeds)
    sketch = bf.CMSketch(rows, cell_width, cells, murmur, cf)
    
    print('inserting...')
    start = time.time()
    sketch.insert_element(id_list, freqs)
    insert_time = time.time() - start
    print('insert time:', insert_time)
    
    print('querying...')
    start = time.time()
    result = sketch.query_element(id_list)
    query_time = time.time() - start
    print('query time:', query_time)
    
    for i in range(30):
        print(result[i], freqs[i])