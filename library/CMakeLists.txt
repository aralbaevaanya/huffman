cmake_minimum_required(VERSION 2.8)

project(huffman)

set(SOURCE_LIB

        huffman.cpp
        huffman.h

        reader.h
        writer.h
        )

add_library(huffman_lib STATIC ${SOURCE_LIB})
