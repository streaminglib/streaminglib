#include "reader.h"

Reader::
Reader(const string &filename, size_t batch_size) {
    this->filename = filename;
    this->batch_size = batch_size;
    this->in.open(filename);
    this->finish = false;
}

vector<string> Reader::
read_batch() {
    vector<string> results;
    string s;
    for (size_t i = 0; i < this->batch_size; i++) {
        if(!(this->in >> s)) {
            this->finish = true;
            break;
        }
        results.push_back(s);
    }
    return results;
}

vector<size_t> Reader::
read_freqs() {
    vector<size_t> results;
    size_t s;
    for (size_t i = 0; i < this->batch_size; i++) {
        if (!(this->in >> s)) {
            this->finish = true;
            break;
        }
        results.push_back(s);
    }
    return results;
}

bool Reader::
finished() const {
    return this->finish;
}

void Reader::
read(const string &filename) {
    if (this->in.is_open())
        this->in.close();
    this->in.open(filename);
    this->finish = false;
}

Reader::
~Reader() {
    if (this->in.is_open())
        this->in.close();
}