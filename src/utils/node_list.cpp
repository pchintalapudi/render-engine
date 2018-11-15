//
// Created by prem on 11/12/2018.
//

#include "include/utils/node_list.h"

void dom::NodeList::erase(const Node *value) {
    auto idx = indexOf(value);
    if (~idx) {
        nodes.erase(nodes.begin() + idx);
        checksum++;
    }
}

dom::Node *dom::NodeList::erase(unsigned long index) {
    Node *val = nodes[index];
    nodes.erase(nodes.begin() + index);
    checksum++;
    return val;
}

std::vector<dom::Node *> dom::NodeList::subvector(unsigned long start, unsigned long end)const {
    return std::vector<Node *>(nodes.begin() + start, nodes.begin() + end);
}

std::vector<dom::Node *> dom::NodeList::erase(unsigned long start, unsigned long end) {
    std::vector<Node *> erased = subvector(start, end);
    nodes.erase(nodes.begin() + start, nodes.begin() + end);
    checksum++;
    return erased;
}

unsigned long dom::NodeList::indexOf(const Node *value) {
    for (unsigned long i = 0; i < nodes.size(); i++) {
        if (nodes[i] == value) {
            return i;
        }
    }
    return ~0;
}

void dom::NodeList::push_back(Node *value) {
    nodes.push_back(value);
    checksum++;
}