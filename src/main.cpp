#include <iostream>
#include <string>

#include "nodes/elements/selectors/css_selector.h"

using namespace std;

int main() {
    string selector;
    getline(cin, selector);
    auto sel = feather::dom::selector::CSSSelector::parse(selector, nullptr);
    cout << sel.toString() << endl;
}