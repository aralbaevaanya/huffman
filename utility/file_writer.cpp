#include <iostream>
#include "file_writer.h"

file_writer::file_writer() : file_name(), writer(), opened(false),
                             begin_index(0), end_index(BUFFER_SIZE) {}

file_writer::file_writer(const std::string &file_name)
        : file_name(file_name), writer(file_name, std::ofstream::binary),
          opened(true), begin_index(0), end_index(BUFFER_SIZE) {

    if (!writer) {
        throw std::invalid_argument("the file can't be opened");
    }
}

file_writer::~file_writer() {
    if (opened) {
        writer.close();
    }
}

bool file_writer::is_opened() {
    return opened;
}

void file_writer::open(const std::string &file_name) {
    if (is_opened()) {
        std::cerr << file_name << " was not opened.\n";
        std::cerr << "The file_reader is already busy with the file: " << this->file_name << std::endl;
        return;
    }
    this->file_name = file_name;
    writer.open(file_name, std::ofstream::binary);
    opened = true;
}

void file_writer::close() {
    if (is_opened()) {
        if (begin_index != 0 && begin_index != BUFFER_SIZE) {
            writer.write(buffer, begin_index);
        }
        begin_index = 0;
        opened = false;
        writer.close();
    }
}

void file_writer::buff_write() {
    if (begin_index == end_index) {
        begin_index = 0;
        writer.write(buffer, BUFFER_SIZE);
    }
}

void file_writer::write(unsigned char ch) {
    buff_write();
    buffer[begin_index++] = ch;
}

file_writer& file_writer::operator<<(unsigned char ch) {
    write(ch);
    return *this;
}

