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
    return np.random.zipf(2, size=size)
    
if __name__ == '__main__':
    test_size = 100000
    rows = 16
    cell_width = 8
    cells = 100000
    
    id_list = rand_id_list(size=test_size)
    freqs = skewed_freq(test_size)

    seeds = range(rows)
    murmur = bf.Murmur3Hash(seeds)
    sketch = bf.CMSketch(rows, cell_width, cells, murmur)
    
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