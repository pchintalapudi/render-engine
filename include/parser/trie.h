//
// Created by binhe on 11/9/2018.
//

#ifndef FEATHER_TRIE_H
#define FEATHER_TRIE_H

#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

namespace feather {
    namespace parser {
        template<typename T, typename Derived>
        class Trie {
        public:
            Derived *feed(char *word, size_t size) {
                return static_cast<Derived*>(this)->feed(word, size, 0);
            }

            Derived *feed(const std::string &word) {
                return static_cast<Derived*>(this)->feed(word, 0);
            }

            void add(const std::string &first, T item) {
                static_cast<Derived*>(this)->add(first, item, 0);
            }

            void add(char *word, size_t size, T item) {
                static_cast<Derived*>(this)->add(word, size, item, 0);
            }

            void remove(const std::string &word) {
                static_cast<Derived*>(this)->remove(word);
            }

            Trie<T, Derived> operator[](const std::string &word) {
                return static_cast<Derived*>(this)->feed(word);
            }
        };

        template<typename T>
        class FlatTrie : public Trie<T, FlatTrie<T>> {
        private:
            std::vector<std::tuple<char *, int>> sequence;
            std::vector<T> elements;
            int currentLocalIndex;
            std::map<char, FlatTrie<T>> map;


            Trie<T, FlatTrie<T>> *feed(char *s, size_t length, size_t index) {
                this->currentLocalIndex += std::min(sequence.size(), length - index);
                this->index += std::min(sequence.size(), length - index);
                if (currentLocalIndex < sequence.size() && index == length - 1) {
                    return this;
                }
                if (currentLocalIndex == sequence.size() - 1 && index < length &&
                    this->map.find(s[index]) != this->map.end()) {
                    return this->map[s[index]].feed(s, length, index);
                }
                return nullptr;
            }
        };

        template<typename T>
        class DefaultTrie : public Trie<T, DefaultTrie<T>> {

        private:
            std::map<char, DefaultTrie<T> *> m;
            bool isHere;
            T t;

        public:

            DefaultTrie<T> *feed(char *c, size_t length, size_t index) {
                if (index == length) {
                    return this;
                }
                if (this->m.find(c[index]) == this->m.end())
                    return nullptr;
                return this->m[c[index]].feed(c, length, index + 1);
            }

            DefaultTrie<T> *feed(const std::string &s, size_t index) {
                if (index == s.length()) {
                    return this;
                }
                if (this->m.find(s[index]) == this->m.end())
                    return nullptr;
                return this->m[s[index]]->feed(s, index + 1);
            }

            void add(const std::string &s, T item, size_t index) {
                if (index == s.length()) {
                    this->t = item;
                    this->isHere = true;
                    return;
                }
                if (this->m.find(s[index]) == this->m.end())
                    this->m[s[index]] = new DefaultTrie<T>();
                this->m[s[index]]->add(s, item, index + 1);
            }

            void add(char *word, size_t size, T item, size_t index) {
                if (index == size) {
                    this->t = item;
                    return;
                }
                if (this->m.find(word[index]) == this->m.end())
                    this->m[word[index]] = new DefaultTrie<T>();
                (*this->m[word[index]]).add(word, item, index + 1);
            }

            void remove(const std::string &word, size_t index) {
                if (index == word.length()) {
                    this->here = false;
                    delete this->t;
                }
            }

            using Trie<T, DefaultTrie<T>>::feed;
            using Trie<T, DefaultTrie<T>>::add;
            using Trie<T, DefaultTrie<T>>::remove;

            DefaultTrie() {
                this->isHere = false;
                this->m = std::map<char, DefaultTrie<T> *>();
            }

            inline T get() {
                return this->t;
            }

            inline bool here() {
                return this->isHere;
            }

        };

    }
}

#endif //FEATHER_TRIE_H
