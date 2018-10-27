#ifndef HUFFMAN1_FILE_READER_H
#define HUFFMAN1_FILE_READER_H

#include <fstream>
#include "reader.h"
#include <string>

struct file_reader : reader {

    file_reader();

    explicit file_reader(const std::string &file_name);

    ~file_reader() override;

    void open(const std::string &file_name);

    void close();

    bool is_opened();

    void buff_read();

    unsigned char next() override;

    bool has_next() override;

    file_reader& operator>>(unsigned char &ch);

private:
    static const size_t BUFFER_SIZE = 131072; // 2^17

    std::string file_name;

    std::ifstream reader;

    bool opened;

    char buffer[BUFFER_SIZE];

    size_t begin_index;

    size_t end_index;

    bool eof;

};


#endif //HUFFMAN1_FILE_READER_H
