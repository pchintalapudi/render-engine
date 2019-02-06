//
// Created by prem on 1/14/2019.
//

#include <iostream>
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

UInt utf8::toCodePoint(const char *c) {
    return toCodePoint(c, charSize(*c));
}

UInt utf8::toCodePoint(const char *c, unsigned char size) {
    UInt codePoint;
    std::memcpy(&codePoint, c, size);
    return codePoint >> sizeof(char) * (sizeof(UInt) - size);
}

void utf8::appendCodePoint(feather::DOMString &str, const char *c) {
    switch (charSize(*c)) {
        case 1:
            str += *c;
            break;
        case 2:
            str += *c;
            str += *(c + 1);
            break;
        case 3:
            str += *c;
            str += *(c + 1);
            str += *(c + 2);
            break;
        case 4:
            str += *c;
            str += *(c + 1);
            str += *(c + 2);
            break;
        default:
            break;
    }
}

void utf8::appendCodePoint(feather::DOMString &str, feather::UInt codePoint) {
    if (codePoint & 0xff000000) goto cp1;
    if (codePoint & 0x00ff0000) goto cp2;
    if (codePoint & 0x0000ff00) goto cp3;
    if (codePoint & 0x000000ff) goto cp4;
    cp1:
    str += char((codePoint >> 3 * 8) & 0xff);
    cp2:
    str += char((codePoint >> 2 * 8) & 0xff);
    cp3:
    str += char((codePoint >> 8) & 0xff);
    cp4:
    str += char(codePoint & 0xff);
}

#ifdef DEBUG_MODE
namespace {
    feather::Vector<std::chrono::high_resolution_clock::time_point> profiles;
}
#endif

UInt profile::profileStart() {
#ifdef DEBUG_MODE
    profiles.push_back(std::chrono::high_resolution_clock::now());
    return profiles.size() - 1;
#else
    return 0;
#endif
}

void profile::profileEnd(feather::UInt id, DOMString name) {
#ifdef DEBUG_MODE
    auto timepoint = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dif = timepoint - profiles[id];
    std::cout << std::endl << dif.count() << " milliseconds were taken by the profile "
              << (name.length() == 0 ? std::to_string(id) : name) << "." << std::endl;
#endif
}

void feather::init() {
    parser::initEscapeCodes();
}

bool utf8::isAscii(char c) {
    return (unsigned char) c >> 7;
}