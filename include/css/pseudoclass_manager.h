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

    void hover(std::vector<dom::Node *> elements);

    void active(std::vector<dom::Node *> elements);

    inline bool isHover(dom::Node *element) { return hovered && moused.find(element) != moused.end(); }

    inline bool isActive(dom::Node *element) { return !hovered && moused.find(element) != moused.end(); }

    void focus(std::vector<dom::Node *> nodes);

    inline bool isFocused(dom::Element *element) { return element == focused; }

    inline bool hasFocus(dom::Element *element) { return focusedNodes.find(element) != focusedNodes.end(); }

private:
    std::set<dom::Node *> moused;
    bool hovered;
    std::set<dom::Node *> focusedNodes;
    dom::Element *focused;
};

#endif //FEATHER_PSEUDOCLASS_MANAGER_H
