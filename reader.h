#ifndef STREAMINGLIB_READER_H
#define STREAMINGLIB_READER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using std::vector;
using std::string;
using std::ifstream;

class Reader {
public:
    Reader(const string &filename, size_t batch_size=2048);
    vector<string> read_batch();
    vector<size_t> read_freqs();
    bool finished() const;
    void read(const string &filename);
    ~Reader();
private:
    string filename;
    size_t batch_size;
    ifstream in;
    bool finish;
};

#endif // STREAMINGLIB_READER_H