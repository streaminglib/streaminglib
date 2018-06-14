#!/usr/bin/python

import string
import random
import time

import numpy as np

import bloom_filter as bf

def get_freq_array(high = 1024, size = 1024):
    return np.random.randint(low = 0, high = 1024, size);


if __name__ == '__main__':
    test_size = 1024;
    domain_size = 1024;
    freq1 = get_freq_array(domain_size, test_size);
    freq2 = get_freq_array(domain_size, test_size);

    start = time.time();
    sketch = bf.skim_sketch(freq1, freq2, domain_size);
    insert_time=  time.time() - start;
    print('insert and calc time is: ', insert_time);

    sketch_res = sketch.join_size;
    exact_res  = np.dot(freq1, freq2);
    error_rate = abs(sketch_res, exact_res) / max(sketch_res, exact_res);

    print('error_rate is: ', error_rate);
