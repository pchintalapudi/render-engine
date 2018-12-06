//
// Created by prem on 11/24/2018.
//

#ifndef FEATHER_PSEUDOCLASS_MANAGER_H
#define FEATHER_PSEUDOCLASS_MANAGER_H

#include <set>
#include <vector>

namespace dom {
    class Element;

    class Node;
}

namespace css {
    class PseudoclassManager;
}

class css::PseudoclassManager {
public:
    inline void clearMoused() { moused.clear(); }

    void hover(const std::vector<const dom::Node *> elements);

    void active(const std::vector<const dom::Node *> elements);

    inline bool isHover(const dom::Node *element) const { return hovered && moused.find(element) != moused.end(); }

    inline bool isActive(const dom::Node *element) const { return !hovered && moused.find(element) != moused.end(); }

    void focus(const std::vector<dom::Node *> nodes);

    inline bool isFocused(const dom::Element *element) const { return element == focused; }

    bool hasFocus(const dom::Element *element) const;

    dom::Element *getFocused() const { return focused; }

private:
    std::set<const dom::Node *> moused;
    bool hovered;
    std::set<const dom::Node *> focusedNodes;
    dom::Element *focused;
};

#endif //FEATHER_PSEUDOCLASS_MANAGER_H
