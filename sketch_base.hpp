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

#endif //STREAMINGLIB_SKETCH_BASE_HPP
