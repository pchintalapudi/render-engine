//
// Created by prem on 12/6/2018.
//
#include "include/css/pseudoclass_manager.h"
#include "include/nodes/element.h"

bool css::PseudoclassManager::hasFocus(const dom::Element *element) const {
    return focusedNodes.find(element) != focusedNodes.end();
}

void css::PseudoclassManager::active(const std::vector<const dom::Node *> elements) {
    moused.clear();
    moused.insert(elements.begin(), elements.end());
    hovered = false;
}

void css::PseudoclassManager::hover(const std::vector<const dom::Node *> elements) {
    moused.clear();
    moused.insert(elements.begin(), elements.end());
    hovered = true;
}

void css::PseudoclassManager::focus(const std::vector<const dom::Node *> nodes) {
    focusedNodes.clear();
    focusedNodes.insert(nodes.begin(), nodes.end());
    for (unsigned long i = nodes.size(); i-- > 0;) {
        if (nodes[i]->getNodeType() == dom::NodeType::ELEMENT_NODE) {
            focused = static_cast<const dom::Element *>(nodes[i]);
            return;
        }
    }
    focused = nullptr;
}