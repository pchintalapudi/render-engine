//
// Created by prem on 12/1/2018.
//

#include "typedefs.h"

std::vector <DOMString> parseStringList(DOMString list) {
    std::vector <DOMString> vec;
    DOMString val = "";
    for (auto character : list) {
        if (isspace(character)) {
            if (!val.empty()) vec.push_back(val);
            val = "";
        } else {
            val += character;
        }
    }
    if (!val.empty()) vec.push_back(val);
    return vec;
}
