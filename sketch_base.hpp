//
// Created by jin on 2018/3/26.
//

#ifndef STREAMINGLIB_SKETCH_BASE_HPP
#define STREAMINGLIB_SKETCH_BASE_HPP

template <typename T>
using hash_ptr = int()(T &);

#define MIN(a, b) (((a) < (b)) ? (a) : (b))


class Sketch
{
protected:
    Sketch();
public:
    virtual ~Sketch() = 0;
};


template <typename T>
class CMSketch: Sketch
{
private:
    int width, depth;
    int **table_ptr;
    hash_ptr<T> *hash_funcs;

public:
    CMSketch(double eps, double delta,
             int width, int depth);
    ~CMSketch();

    void update(T &item, int num);
    int point_query(T &item) const;

    int get_width() const;
    int get_depth() const;
    void set_hash(hash_ptr *hash_ptrs);

    static const double default_delta;
    static const double default_eps;
};

class Hash
{
private:
    int **params;
    int w, d;
    bool use_external_buffer;
    static const int prime_num = 95;
    static const int max_prime = 499;
    static const int prime_numbers[prime_num] =
            {
                    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
                    37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
                    79, 83, 89, 97, 101, 103, 107, 109, 113,
                    127, 131, 137, 139, 149, 151, 157, 163,
                    167, 173, 179, 181, 191, 193, 197, 199,
                    211, 223, 227, 229, 233, 239, 241, 251,
                    257, 263, 269, 271, 277, 281, 283, 293,
                    307, 311, 313, 317, 331, 337, 347, 349,
                    353, 359, 367, 373, 379, 383, 389, 397,
                    401, 409, 419, 421, 431, 433, 439, 443,
                    449, 457, 461, 463, 467, 479, 487, 491, 499,
            };
public:
    Hash(int w, int d);
    ~Hash();
    void hash(int key, int *values) const;
};

int Hash::

#endif //STREAMINGLIB_SKETCH_BASE_HPP
