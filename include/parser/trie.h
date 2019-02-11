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
                return static_cast<Derived *>(this)->feed(word, size, 0);
            }

            Derived *feed(const std::string &word) {
                return static_cast<Derived *>(this)->feed(word, 0);
            }

            void add(const std::string &first, T item) {
                static_cast<Derived *>(this)->add(first, item, 0);
            }

            void add(char *word, size_t size, T item) {
                static_cast<Derived *>(this)->add(word, size, item, 0);
            }

            void remove(const std::string &word) {
                static_cast<Derived *>(this)->remove(word);
            }

            Trie<T, Derived> operator[](const std::string &word) {
                return static_cast<Derived *>(this)->feed(word);
            }
        };

        /**
         * FlatTrie represents a more space efficient representation of the Trie at the cost of computational
         * complexity. It simply adds a size_t to represent the local state.
         * @tparam T The type that the Trie holds
         */
        template<typename T>
        class FlatTrie : public Trie<T, FlatTrie<T>> {
        private:
            // Item if it exists will be at the last element
            T item;
            bool isHere;
            char *sequence;

            size_t length;
            size_t currentLocalIndex;

            std::map<char, FlatTrie<T>> map;

            void clear() {
                this->currentLocalIndex = 0;
            }

            FlatTrie<T> *feed(char *s, size_t length, size_t index) {
                // We first need to determine how far the currentLocalIndex advances. This means we first
                // look at the minimum of the remaining length and the size of the sequence. If the remaining characters
                // is smaller, we should increment simply by the length of the sequence as we will not overflow this
                // node. Otherwise, we should simply finish off the remaining sequence.

                auto cmpLength = std::min(sequence.size() - this->currentLocalIndex, length - index);

                if (!strcmp(sequence, s + index, cmpLength)) {
                    return nullptr;
                }

                this->currentLocalIndex += cmpLength;
                index += cmpLength;

                if (this->currentLocalIndex < sequence.size() && index == length - 1) {
                    return this;
                }

                if (currentLocalIndex == sequence.size() - 1 && index < length &&
                    this->map.find(s[index]) != this->map.end()) {
                    auto e = this->map[s[index]].feed(s, length, index + 1);
                    e.clear();
                    return;
                }

                return nullptr;
            }

            FlatTrie<T> *feed(const std::string &s, size_t index) {
                return nullptr;
            }

            void add(const std::string &s, T item, size_t index) {
                if (index == s.length()) {
                    this->t = item;
                    this->isHere = true;
                    return;
                }
                if (this->m.find(s[index]) == this->m.end())
                    this->m[s[index]] = new FlatTrie<T>();
                this->m[s[index]]->add(s, item, index + 1);
            }

            void split(size_t index) {

            }


            T get() {
                return this->item;
            }

            bool here() {
                return this->isHere;
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

            bool remove(const std::string &word, size_t index) {
                if (index == word.length()) {
                    this->here = false;
                    delete this->t;
                } else {

                }
                return this->m.empty();
            }

            using Trie<T, DefaultTrie<T>>::feed;
            using Trie<T, DefaultTrie<T>>::add;
            using Trie<T, DefaultTrie<T>>::remove;

            DefaultTrie() {
                isHere = false;
                this->m = std::map<char, DefaultTrie<T> *>();
            }

            inline T get() {
                return this->t;
            }

        };

    }
}

#endif //FEATHER_TRIE_H
