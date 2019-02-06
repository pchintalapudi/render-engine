//
// Created by binhe on 11/9/2018.
//

#ifndef FEATHER_TRIE_TEST_H
#define FEATHER_TRIE_TEST_H
#include "gtest/gtest.h"
#include "parser/trie.h"

class TrieTest : public ::testing::Test {
protected:
    feather::parser::DefaultTrie<bool> emptyTrie;
    feather::parser::DefaultTrie<bool> trie;
};


#endif //FEATHER_TRIE_H
