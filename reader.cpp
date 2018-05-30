//
// Created by jin on 2018/5/10.
//

#include "reader.h"


FileReader::
FileReader(const string &filename,
           const Transformer &transformer,
           size_t batch) {
    this->in.open(filename);
    this->batch_size = batch;
    this->transformer = transformer;
}

FileReader::
~FileReader() {
    this->in.close();
}

void FileReader::
next_batch(vector<string> &out) const {
    if (!out.empty()) out.clear();
    string next_str;
    for (size_t i = 0; i < batch_size; i++) {
        in >> next_str;
        if (next_str.empty()) return;
        out.push_back(transformer.transform(next_str));
    }
}