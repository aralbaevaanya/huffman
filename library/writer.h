#ifndef HUFFMAN1_WRITER_H
#define HUFFMAN1_WRITER_H

#include <fstream>
#include <cstdlib>

struct writer {
    virtual ~writer() {}

    virtual void write(unsigned char ch) = 0;

    virtual writer& operator<<(unsigned char ch) = 0;
};

#endif //HUFFMAN1_WRITER_H
