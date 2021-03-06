#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "huffman.h"

huffman::huffman() : counter(ALPHABET_SIZE), leaves(ALPHABET_SIZE), words(ALPHABET_SIZE) {}

void huffman::count(reader &in) {
    while (in.has_next()) {
        counter[in.next()]++;
    }
}

huffman::tree_node::tree_node(uint16_t ch, uint64_t ch_count)
        : character(ch), count(ch_count) {}

void huffman::delete_dfs(tree_node *node) {
    if (node != nullptr) {
        delete_dfs(node->left);
        delete_dfs(node->right);
        delete (node);
    }
}

void huffman::build_tree() {
    delete_dfs(root);
    root = nullptr;
    std::vector<tree_node *> chars_sorted(ALPHABET_SIZE);
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        chars_sorted[i] = new tree_node(i, counter[i]);
    }
    std::sort(chars_sorted.begin(), chars_sorted.end(),
              [](tree_node *a, tree_node *b) ->
                      bool { return a->count < b->count; });

    std::vector<tree_node *> sums_sorted;
    size_t min_char = 0, min_sum = 0, max_sum = 0;

    while (chars_sorted[min_char]->count == 0) {
        delete (chars_sorted[min_char]);
        min_char++;
    }

    for (;;) {
        uint64_t sum1 = UINT64_MAX, sum2 = UINT64_MAX, sum3 = UINT64_MAX;
        if (min_char + 1 < ALPHABET_SIZE) {
            sum1 = chars_sorted[min_char]->count + chars_sorted[min_char + 1]->count;
        }
        if (min_char < ALPHABET_SIZE && min_sum < max_sum) {
            sum2 = chars_sorted[min_char]->count + sums_sorted[min_sum]->count;
        }
        if (min_sum + 1 < max_sum) {
            sum3 = sums_sorted[min_sum]->count + sums_sorted[min_sum + 1]->count;
        }
        if (sum1 == UINT64_MAX && sum2 == UINT64_MAX && sum3 == UINT64_MAX) {
            break;
        } else if (sum1 <= sum2 && sum1 <= sum3) {
            tree_node *new_node = new tree_node(ALPHABET_SIZE, sum1);
            sums_sorted.push_back(new_node);
            new_node->left = chars_sorted[min_char];
            new_node->right = chars_sorted[min_char + 1];
            min_char += 2;
        } else if (sum2 <= sum1 && sum2 <= sum3) {
            tree_node *new_node = new tree_node(ALPHABET_SIZE, sum2);
            sums_sorted.push_back(new_node);
            new_node->left = chars_sorted[min_char];
            new_node->right = sums_sorted[min_sum];
            min_char++;
            min_sum++;
        } else if (sum3 <= sum1 && sum3 <= sum2) {
            tree_node *new_node = new tree_node(ALPHABET_SIZE, sum3);
            sums_sorted.push_back(new_node);
            new_node->left = sums_sorted[min_sum];
            new_node->right = sums_sorted[min_sum + 1];
            min_sum += 2;
        }
        max_sum++;
    }
    root = sums_sorted.back();

    int8_t leaf_counter = 0;
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        leaves[i] = ALPHABET_SIZE - 1;
    }
    words_dfs(root, new std::vector<bool>, leaf_counter);
}

void huffman::words_dfs(tree_node *node, std::vector<bool> *cur, int8_t &leaf_counter) {
    if (node->left == nullptr) {
        leaves[node->character] = leaf_counter;
        leaf_counter++;
        words[node->character].resize(cur->size());
        for (size_t i = 0; i < cur->size(); i++) {
            words[node->character][i] = (*cur)[i];
        }
    } else {
        cur->push_back(1);
        words_dfs(node->left, cur, leaf_counter);
        cur->pop_back();
        cur->push_back(0);
        words_dfs(node->right, cur, leaf_counter);
        cur->pop_back();
    }
}

void huffman::write_tree(writer &out) {
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        out << leaves[i];
    }
    std::vector<bool> written_tree;
    write_dfs(root, &written_tree);
    while (written_tree.size() % 8 != 0) {
        written_tree.push_back(0);
    }
    out << written_tree.size() / 8;
    for (size_t i = 0; i < written_tree.size(); i += 8) {
        uint8_t ch = 0;
        for (size_t j = 0; j < 8; j++) {
            ch <<= 1;
            if (written_tree[i + j]) {
                ch += 1;
            }
        }
        out << ch;
    }
}

void huffman::write_dfs(tree_node *node, std::vector<bool> *written_tree) {
    if (node->left != nullptr) {
        written_tree->push_back(1);
        write_dfs(node->left, written_tree);
        write_dfs(node->right, written_tree);
    }
    written_tree->push_back(0);
}

void huffman::compress(reader &in, writer &out) {
    uint16_t number_of_symbols = 0;
    uint8_t last = 0;
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        if (counter[i] != 0) {
            number_of_symbols++;
            last = i;
        }
    }

    if (number_of_symbols == 0) {
        // just nothing
    } else if (number_of_symbols == 1) {
        out << 1 << last;
        for (int16_t i = 56; i >= 0; i -= 8) {
            out << (counter[last] >> i) % 256;
        }
    } else {
        out << 0;
        build_tree();

        write_tree(out);

        unsigned char cur_buffer = 0;
        uint8_t buffer_fullness = 0;

        for (;;) {
            uint16_t ch;
            if (in.has_next()) {
                ch = in.next();
            } else {
                cur_buffer <<= 1;
                buffer_fullness++;
                if (buffer_fullness == 8) {
                    out << cur_buffer;
                    cur_buffer = 0;
                    buffer_fullness = 0;
                }
                break;
            }
            for (size_t i = 0; i < words[ch].size(); i++) {
                cur_buffer <<= 1;
                if (words[ch][i]) {
                    cur_buffer++;
                }
                buffer_fullness++;
                if (buffer_fullness == 8) {
                    out << cur_buffer;
                    cur_buffer = 0;
                    buffer_fullness = 0;
                }
            }
        }
        uint8_t bits_counter = 0;
        if (buffer_fullness != 0) {
            while (buffer_fullness < 8) {
                cur_buffer <<= 1;
                bits_counter++;
                buffer_fullness++;
            }
            out << cur_buffer;
        }
        out << 8 - bits_counter-1;
    }
}

void huffman::read_tree(reader &in) {
    if (root != nullptr) {
        delete_dfs(root);
        root = nullptr;
    }
    std::vector<uint8_t> back_leaves(ALPHABET_SIZE);
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        back_leaves[i] = ALPHABET_SIZE - 1;
    }
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        uint8_t t = in.next();
        back_leaves[t] = i;
    }

    int16_t tree_size = in.next();
    tree_node *cur_node = root = new tree_node(ALPHABET_SIZE, 0);
    uint8_t cur_leaf = 0;
    for (size_t i = 0; i < tree_size; i++) {
        uint8_t ch = static_cast<uint8_t>(in.next());
        uint8_t reversed_ch = 0;
        for (uint8_t j = 0; j < 8; j++) {
            reversed_ch <<= 1;
            if (ch % 2 == 1) {
                reversed_ch++;
            }
            ch >>= 1;
        }
        ch = reversed_ch;

        for (int8_t j = 0; j < 8; j++) {
            if (ch % 2 == 1) {
                cur_node->left = new tree_node(ALPHABET_SIZE, 0);
                cur_node->left->parent = cur_node;
                cur_node = cur_node->left;
            } else {
                if (cur_node->left == nullptr) {
                    cur_node->character = back_leaves[cur_leaf];
                    cur_leaf++;
                }
                if (cur_node->parent == nullptr) {
                    return;
                } else if (cur_node->parent->right == cur_node) {
                    cur_node = cur_node->parent;
                } else {
                    cur_node->parent->right = new tree_node(ALPHABET_SIZE, 0);
                    cur_node->parent->right->parent = cur_node->parent;
                    cur_node = cur_node->parent->right;
                }
            }
            ch /= 2;
        }
    }
}

void huffman::debug_write_tree(tree_node *node, std::string s) {
    if (node->character != 256) {
        std::cout << node->character << " " << static_cast<char>(node->character) << std::endl;
    }
    if (node->left) {
        std::cout << s << "l" << std::endl;
        debug_write_tree(node->left, s + " ");
        std::cout << s << "r" << std::endl;
        debug_write_tree(node->right, s + " ");
    }
}

void huffman::decompress(reader &in, writer &out) {
    if (!in.has_next()) {
        return;
    }
    unsigned char mode = in.next();
    if (mode > 0) {
        unsigned char ch = in.next();
        uint64_t count = 0;
        // read 64
        for (uint8_t i = 0; i < 8; i++) {
            count <<= 1;
            count += static_cast<uint64_t>(in.next());
        }
        for (uint64_t i = 0; i < count; i++) {
            out << ch;
        }
    } else {
        read_tree(in);
        //debug_write_tree(root, "");

        tree_node *cur_node = root;
        uint8_t ch1 = static_cast<uint8_t>(in.next());
        while (in.has_next()) {
            uint8_t ch2 = static_cast<uint8_t>(in.next());
            uint8_t bits_counter = 8;
            if (!in.has_next()) {
                bits_counter = ch2;
            }
            
            uint8_t reversed_ch = 0;
            for (uint8_t j = 0; j < 8; j++) {
                reversed_ch <<= 1;
                if (ch1 % 2 == 1) {
                    reversed_ch++;
                }
                ch1 >>= 1;
            }
            ch1 = reversed_ch;

            int8_t max_bit = 8;
            if (!in.has_next()) {
                max_bit = bits_counter;
            }

            for (int8_t i = 0; i < max_bit; i++) {
                if (ch1 % 2 == 1) {
                    cur_node = cur_node->left;
                } else {
                    cur_node = cur_node->right;
                }
                if (cur_node->character < ALPHABET_SIZE) {
                    out << static_cast<unsigned char>(cur_node->character);
                    cur_node = root;
                }
                ch1 /= 2;
            }

            ch1 = ch2;
        }

    }
}
