//
// Created by binhe on 11/9/2018.
//

#ifndef FEATHER_TRIE_H
#define FEATHER_TRIE_H

#include <string>
#include <unordered_map>
#include <vector>

namespace feather {
    namespace parser {
        template<class T>
        class Trie {
        private:
            std::unordered_map<char, Trie<T> *> m;

            Trie<T> *feed(char *, unsigned int, unsigned int);

            Trie<T> *feed(std::string, unsigned int);

            void add(std::string, T, unsigned int);

            void remove(std::string, unsigned int);

            void split(int);

            std::vector<int> here;
            char* internal;
            T t;

        public:
            Trie() {
                this->m = std::unordered_map<char, Trie<T> *>();
            };

            Trie<T> *feed(char c);

            Trie<T> *feed(char *c, unsigned int n);

            Trie<T> *feed(std::string);

            void add(std::string, T);

            void remove(std::string);

            T get();

        };
    }
}

#include "Trie.hpp"

#endif //FEATHER_TRIE_H
