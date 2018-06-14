#!/usr/bin/python

import time

import bloom_filter as bf

if __name__ == '__main__':
    seeds = [2, 3, 5, 7]
    sketch = bf.ams_sketch(4, 4, seeds)
    s = "input"

    start = time.time()
    sketch.insert_element(s, 1)
    insert_time = time.time() - start
    print('insert time:', insert_time)
    
    start = time.time()
    result = sketch.obj_count(s)
    query_time = time.time() - start
    print('query time:', query_time)
    
    print('query result:', result)