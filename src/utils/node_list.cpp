//
// Created by prem on 11/12/2018.
//

#include "include/utils/node_list.h"

void NodeList::erase(const Node *value) {
    auto idx = indexOf(value);
    if (~idx) {
        nodes.erase(nodes.begin() + idx);
    }
}

Node *NodeList::erase(unsigned long index) {
    Node *val = nodes[index];
    nodes.erase(nodes.begin() + index);
    checksum++;
    return val;
}

std::vector<Node *> NodeList::subvector(unsigned long start, unsigned long end) {
    return std::vector<Node *>(nodes.begin() + start, nodes.begin() + end);
}

std::vector<Node *> NodeList::erase(unsigned long start, unsigned long end) {
    std::vector<Node *> erased = subvector(start, end);
    nodes.erase(nodes.begin() + start, nodes.begin() + end);
    checksum++;
    return erased;
}

unsigned long NodeList::indexOf(const Node *value) {
    for (unsigned long i = 0; i < nodes.size(); i++) {
        if (nodes[i] == value) {
            return i;
        }
    }
    return ~0;
}

void NodeList::push_back(Node *value) {
    nodes.push_back(value);
    checksum++;
}