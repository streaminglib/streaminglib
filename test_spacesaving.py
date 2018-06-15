import string
import random
import time

import numpy as np

import bloom_filter as bf

def rand_id_list(len=1000):
    return np.random.randint(0, 1024, len);

def skewed_freq(len=1000):
    return 20 * np.random.zipf(2, len);

if __name__ == '__main__':
    sketch = bf.spacesaving(31);

    rand_array = rand_id_list(10240);
    rand_freq = skewed_freq(10240);

    for i in range(0, 10240):
        sketch.insert(rand_array[i], rand_freq[i]);

    res = sketch.query_top();
