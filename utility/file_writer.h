#ifndef HUFFMAN1_FILE_WRITER_H
#define HUFFMAN1_FILE_WRITER_H

#include <cstdlib>
#include <string>
#include <fstream>
#include "writer.h"

struct file_writer : writer {

    file_writer();

    explicit file_writer(const std::string &file_name);

    file_writer& operator=(const file_writer& other) = delete;

    ~file_writer();

    bool is_opened();

    void open(const std::string &file_name);

    void close();

    void buff_write();

    void write(unsigned char ch);

    file_writer& operator<<(unsigned char ch);

private:
    static const size_t BUFFER_SIZE = 131072; // 2^17

    std::string file_name;

    std::ofstream writer;

    bool opened;

    char buffer[BUFFER_SIZE];

    size_t begin_index;

    size_t end_index;

};

#endif //HUFFMAN1_FILE_WRITER_H
