//
// Created by jin on 2018/5/10.
//

#ifndef STREAMINGLIB_READER_H
#define STREAMINGLIB_READER_H

#include <string>
#include <fstream>
#include <vector>
using std::string;
using std::ifstream;
using std::vector;

class Transformer
{
public:
    virtual string transform(const string &s) const = 0;
};

class Reader
{
public:
    virtual void next_batch(vector<string> &out) const = 0;
};

class FileReader: public Reader
{
public:
    FileReader(const string &filename,
               const Transformer &transformer,
               size_t batch);
    void next_batch(vector<string> &out) const override;
    ~FileReader();
private:
    ifstream in;
    size_t batch_size;
    Transformer transformer;
};

#endif //STREAMINGLIB_READER_H
