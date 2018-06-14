#!/usr/bin/python

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
    test_size = 1000
    seeds = [2, 3, 5, 7]
    # 把 sketch 大小修改到与 testset 相适应的大小
    sketch = bf.ams_sketch(4, 4, seeds)
    
    id_list = rand_id_list(size=test_size)
    freqs = skewed_freq(test_size)

    start = time.time()
    sketch.insert_element(id_list, freqs)
    insert_time = time.time() - start
    print('insert time:', insert_time)
    
    start = time.time()
    result = sketch.obj_count(id_list)
    query_time = time.time() - start
    print('query time:', query_time)
    
    print('query result:')
    for est, truth in zip(result, freqs):
        print(est, '-', truth)
    