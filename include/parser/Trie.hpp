#include "Trie.h"

#ifdef FEATHER_TRIE_H
using namespace feather::parser;

template<class T>
Trie<T> *Trie<T>::feed(char *c, unsigned int n) {
    return this->feed(c, n, 0);
}

template<class T>
Trie<T> *Trie<T>::feed(char c) {
    if (this->m.find(c) == this->m.end())
        return nullptr;
    return this->m[c];
}

template<class T>
Trie<T> *Trie<T>::feed(std::string s) {
    return this->feed(s, 0);
}

template<class T>
void Trie<T>::add(std::string s, T t) {
    return this->add(s, t, 0);
}

template<class T>
void Trie<T>::add(std::string s, T t, unsigned int index) {
    if (index == s.length()) {
        this->t = t;
        return;
    }
    if (this->m.find(s[index]) == this->m.end())
        this->m[s[index]] = new Trie<T>();
    (*this->m[s[index]]).add(s, t, index + 1);
}

template<class T>
void Trie<T>::remove(std::string s) {
    this->remove(s, 0);
}

template <class T>
void Trie<T>::remove(std::string s, unsigned int index) {
    if (index == s.length()) {
        this->here = false;
    }
}

template<class T>
Trie<T> *Trie<T>::feed(char *c, unsigned int length, unsigned int index) {
    if (index == length) {
        return this;
    }
    if (this->m.find(c[index]) == this->m.end())
        return nullptr;
    return this->m[c[index]].feed(c, t, index + 1);
}

template<class T>
Trie<T> *Trie<T>::feed(std::string s, unsigned int index) {
    if (index == s.length()) {
        return this;
    }
    if (this->m.find(s[index]) == this->m.end())
        return nullptr;
    return this->m[s[index]]->feed(s, index + 1);
}

template<class T>
T Trie<T>::get() {
    return this->t;
}


#endif