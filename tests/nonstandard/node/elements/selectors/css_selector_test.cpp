//
// Created by prem on 1/14/2019.
//

#include "nodes/elements/selectors/css_selector.h"
#include "css_selector_test.h"

namespace {
    const char *tests[] = {"a>b>c", "a > b > c", "a+b>c~d", "a + b>c ~ d", "a b c d", "a b> c + d"};
}

void runParseTests() {
}