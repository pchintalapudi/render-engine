//
// Created by binhe on 11/9/2018.
//

#ifndef FEATHER_TRIE_H
#define FEATHER_TRIE_H

#include <string>
#include <unordered_map>

namespace feather {
    namespace parser {
        template<class T>
        class Trie {
        private:
            std::unordered_map<char, Trie<T>> m;

            Trie<T> feed(char *, unsigned int, unsigned int);

            Trie<T> feed(std::string, unsigned int);

            void add(std::string, T, unsigned int);

            T t;

        public:
            Trie() {
                this->m = std::unordered_map<char, Trie<T>>();
                this->t = nullptr;
            };

            Trie<T> feed(char *c, unsigned int n);

            Trie<T> feed(std::string);

            void add(std::string, T);

            T get();

        };
    }
}

#include "Trie.hpp"

#endif //FEATHER_TRIE_H
