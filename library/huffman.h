#ifndef HUFFMAN1_HUFFMAN_H
#define HUFFMAN1_HUFFMAN_H

#include <vector>
#include "reader.h"
#include "writer.h"

class huffman {
    const uint16_t ALPHABET_SIZE = 256;
    std::vector<uint64_t> counter;

    struct tree_node {
        uint16_t character;
        uint64_t count;
        tree_node *parent = nullptr,
                *left = nullptr,
                *right = nullptr;

        tree_node(uint16_t ch, uint64_t count);
    };

    tree_node *root = nullptr;

    std::vector<uint8_t> leaves;
    std::vector<std::vector<bool>> words;

    void delete_dfs(tree_node *);

    void words_dfs(tree_node *, std::vector<bool> *, int8_t &);

    void write_dfs(tree_node *, std::vector<bool> *);

    void build_tree();

    void write_tree(writer &);

    void read_tree(reader &);

    void debug_write_tree(tree_node *, std::string);

public:
    huffman();

    void count(reader &);

    void compress(reader &, writer &);

    void decompress(reader &, writer &);

};

#endif //HUFFMAN1_HUFFMAN_H
