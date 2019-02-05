//
// Created by binhe on 11/9/2018.
//

#include "TrieTest.h"
using namespace feather::parser;

// NOLINT
TEST_F(TrieTest, TestFeed) {
    emptyTrie.get();
}

TEST_F(TrieTest, TestAdd) {
    trie.add(std::string("word"), true);
    trie.add(std::string("some other word"), true);
    trie.add(std::string("water"), true);
    trie.add(std::string("waiter"), true);
    trie.add(std::string("hello"), true);
}

TEST_F(TrieTest, TestGet) {
    trie.add(std::string("word"), true);
    trie.add(std::string("some other word"), true);
    trie.add(std::string("water"), true);
    trie.add(std::string("waiter"), true);
    ASSERT_TRUE(trie.feed("word")->get());
    ASSERT_TRUE(trie.feed("water")->get());
    ASSERT_EQ(trie.feed("beauty"), nullptr);
    ASSERT_EQ(trie.feed("world"), nullptr);
}
