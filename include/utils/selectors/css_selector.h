//
// Created by prem on 11/15/2018.
//

#ifndef FEATHER_CSS_SELECTOR_H
#define FEATHER_CSS_SELECTOR_H

#include <deque>
#include "css_selector_token_group.h"

namespace css {
    class CSSSelector;
}

class css::CSSSelector {
public:

    CSSSelector(std::vector<CSSSelectorTokenGroup *> groups);

    bool matches(const dom::Element *element) const;

    inline unsigned long size() { return groups.size(); }

    void preprocess(const dom::Element *element);

    CSSSelectorTokenGroup *process(const dom::Element *element);

    inline void push(CSSSelectorTokenGroup *group) { groups.push_front(group); }

    DOMString toString() const;

    ~CSSSelector();

private:
    std::deque<CSSSelectorTokenGroup *> groups;
};

#endif //FEATHER_CSS_SELECTOR_H
