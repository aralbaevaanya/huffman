#ifndef HUFFMAN1_READER_H
#define HUFFMAN1_READER_H

#include <fstream>
#include <cstdlib>

struct reader {
    virtual ~reader() {}

    virtual unsigned char next() = 0;

    virtual bool has_next() = 0;

    virtual reader& operator>>(unsigned char &ch) = 0;
};

#endif //HUFFMAN1_READER_H
