cmake_minimum_required(VERSION 2.8)

project(utility)

include_directories(${huffman_SOURCE_DIR})

set(UTIL_LIB

        main.cpp

        file_huffman.cpp
        file_huffman.h

        file_reader.cpp
        file_reader.h

        file_writer.cpp
        file_writer.h
        )

add_executable(Huffman ${UTIL_LIB})

target_link_libraries(Huffman huffman_lib)
