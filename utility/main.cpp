#include <iostream>
#include <cassert>
#include "file_huffman.h"
#include "file_reader.h"
#include "file_writer.h"

bool compare_files(std::string const &file1, std::string const &file2) {
    bool eq = true;
    file_reader reader1(file1);
    file_reader reader2(file2);
    for (uint64_t i = 0; reader1.has_next() && reader2.has_next(); i++) {
        unsigned char ch1 = reader1.next(), ch2 = reader2.next();
        //       std::cout << ch1 << " " << ch2 << std::endl;
        if (ch1 != ch2) {
            eq = false;
            /*std::cout << "error: \"" << i << "\" byte - file1 \"" <<
                      static_cast<uint32_t>(ch1) << "\", file2 \"" <<
                      static_cast<uint32_t>(ch2) << "\"" << std::endl;
            */
        }
    }
    if (reader1.has_next() != reader2.has_next()) {
        while (reader1.has_next()) {
            std::cout << "1" << reader1.next() << std::endl;
        }
        while (reader2.has_next()) {
            std::cout << "2" << reader2.next() << std::endl;
        }
        eq = false;
        std::cout << "error: file have different length" << std::endl;
    }
    reader1.close();
    reader2.close();
    if (!eq) {
        std::cout << "error: files \"" << file1 << "\" and \"" << file2 << "\" are not equal" << std::endl;
    }
    return eq;
}

bool test(std::string fileName, std::string format) {
    std::cout << "test " << fileName << format << std::endl;
    compr_or_decompr_file(true, fileName + format, fileName + "_compressed" + format);
    compr_or_decompr_file(false, fileName + "_compressed" + format, fileName + "_decompressed" + format);
    if (compare_files(fileName + format, fileName + "_decompressed" + format)) {
        std::cout << "correct" << std::endl;
    } else {
        std::cout << "wrong" << std::endl;
        assert(false);
    }
    std::cout << "________________________\n" << std::endl;
}

void generate_file(std::string fileName, std::string format, uint32_t length) {
    file_writer writer(fileName + format);
    for (uint32_t i = 0; i < length; i++) {
       // uint32_t a;
        writer.write(static_cast<char>(rand() % 256));
    }
    writer.close();
}

int main(int argc, char **argv) {

    if (argc == 1) {
        std::cerr << "Please, enter <compress|decompress> <input_file_name> <output_file_name> "
                "or enter <test>" << std::endl;
        return 0;
    }
    std::string mode = argv[1];

    if (mode == "test") {

        test("../../tests/test1", ".txt");
        test("../../tests/test2", ".txt");
        test("../../tests/test3", ".7z");
        test("../../tests/unicode_start", "");

        generate_file("../../tests/file", "", 0);
        test("../../tests/file", "");

        for (uint32_t i = 0; i < 200; i++) {
            std::cout <<"10:\t"<< i << std::endl;
            generate_file("../../tests/file", "", 10);
            test("../../tests/file", "");
        }

        for (uint32_t i = 0; i < 50; i++) {
            std::cout <<"100:\t"<< i << std::endl;
            generate_file("../../tests/file", "", 100);
            test("../../tests/file", "");
        }

        for (uint32_t i = 0; i < 5; i++) {
            std::cout <<"1000:\t"<< i << std::endl;
            generate_file("../../tests/file", "", 1000);
            test("../../tests/file", "");
        }

    } else {
        if (argc == 4) {
            if (mode == "compress" || mode == "decompress") {
                compr_or_decompr_file(mode == "compress", argv[2], argv[3]);
                return 0;
            }
        }
        std::cerr << "Please, enter <compress|decompress> <input_file_name> <output_file_name> "
                "or enter <test>" << std::endl;

    }

    return 0;
}
