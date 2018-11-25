#include <iostream>
#include <string>
#include "include/utils/selectors/css_selector.h"

using namespace std;

namespace css {
    CSSSelector parse(DOMString selector);
}

int main()
{
    string selector;
    getline(cin, selector);
    auto cssSelector = css::parse(selector);
    cout << cssSelector.toString() << " printed " <<endl;
}