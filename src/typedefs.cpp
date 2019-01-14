//
// Created by prem on 1/14/2019.
//

#include "typedefs.h"

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