//
// Created by prem on 1/14/2019.
//

#include "typedefs.h"

using namespace feather;

namespace {
    const feather::StrongPointer<void> empty{};
    const feather::DOMString str{};
}

template<typename T>
const feather::StrongPointer<T> &feather::getEmptyPtr() {
    return std::static_pointer_cast<T>(empty);
}

const feather::DOMString &feather::getEmptyString() {
    return str;
}

unsigned char utf8::charSize(char c) {
    switch ((unsigned char) c >> (sizeof(char) * 8 - 4)) {
        default:
            return 1;
        case 0b1101:
        case 0b1100:
            return 2;
        case 0b1110:
            return 3;
        case 0b1111:
            return 4;
    }
}