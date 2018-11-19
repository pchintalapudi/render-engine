//
// Created by prem on 11/12/2018.
//

#include <sstream>
#include "include/utils/dom_token_list.h"

DOMString dom::DOMTokenList::_get_cacheable_value() const {
    std::stringstream stream;
    if (backing.size()) {
        stream << backing[0];
    }
    std::for_each(backing.begin() + 1, backing.end(), [&stream](auto token) { stream << token; });
    return stream.str();
}

void dom::DOMTokenList::remove(DOMString val) {
    for (int i = 0; i < backing.size(); i++) {
        if (backing[i] == val) {
            backing.erase(backing.begin() + i);
            checksum++;
            return;
        }
    }
}

void dom::DOMTokenList::replace(DOMString replacement, DOMString target) {
    for (unsigned long i = 0; i < backing.size(); i++) {
        if (backing[i] == target) {
            backing[i] == replacement;
            checksum++;
            return;
        }
    }
}

bool dom::DOMTokenList::toggle(DOMString token) {
    for (unsigned long i = 0; i < backing.size(); i++) {
        if (backing[i] == token) {
            backing.erase(backing.begin() + i);
            return false;
        }
    }
    backing.push_back(token);
    return true;
}