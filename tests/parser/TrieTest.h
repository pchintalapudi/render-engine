//
// Created by binhe on 11/9/2018.
//

#ifndef FEATHER_TRIE_TEST_H
#define FEATHER_TRIE_TEST_H
#include "gtest/gtest.h"
#include "parser/Trie.h"

class TrieTest : public ::testing::Test {
protected:
    feather::parser::Trie<bool> emptyTrie;
    feather::parser::Trie<bool> trie;

    void SetUp() override {
        trie.add(std::string("word"), true);
        trie.add(std::string("some other word"), true);
        trie.add(std::string("water"), true);
        trie.add(std::string("waiter"), true);
    }
};


#endif //FEATHER_TRIE_H