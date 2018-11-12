//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_NODE_LIST_H
#define FEATHER_NODE_LIST_H

#include <algorithm>
#include <vector>

class Node;

class NodeList {
public:
    inline Node *get(unsigned long index) const {
        return nodes[index];
    }

    inline Node *set(unsigned long index, Node *value) {
        nodes[index] = value;
        checksum++;
    }

    void erase(const Node *value);

    unsigned long indexOf(const Node *value);

    std::vector<Node *> erase(unsigned long start, unsigned long end);

    Node *erase(unsigned long index);

    inline Node *erase(int index) { return erase(static_cast<unsigned long>(index)); }

    std::vector<Node *> subvector(unsigned long start, unsigned long end);

    void push_back(Node *value);

    inline void insert(unsigned long index, Node *value) {
        nodes.insert(nodes.begin() + index, value);
        checksum++;
    }

    inline void insert(unsigned long index, std::vector<Node *> &values) {
        nodes.insert(nodes.begin() + index, values.begin(), values.end());
        checksum++;
    }

    template<typename Op>
    inline void forEach(Op op) const {
        std::for_each(nodes.begin(), nodes.end(), op);
    }

    template<typename Op>
    inline void forEach(unsigned long start, unsigned long end, Op op) {
        std::for_each(nodes.begin() + start, nodes.begin() + end, op);
    }

    template<typename Mapper, typename T>
    std::vector<T> map(Mapper mapper) const {
        std::vector<T> vec;
        for (Node *node : nodes) {
            vec.push_back(mapper(node));
        }
        return vec;
    }

    template<typename Filter>
    std::vector<Node *> filter(Filter filter) const {
        std::vector<Node *> vec;
        for (Node *node : nodes) {
            if (filter(node)) vec.push_back(node);
        }
        return vec;
    }

    template<typename Filter>
    bool anyMatch(Filter filter) const {
        for (Node *node : nodes) {
            if (filter(node)) return true;
        }
        return false;
    }

    template<typename Filter>
    inline bool allMatch(Filter filter) {
        return anyMatch([filter](Node *node) { return !filter(node); });
    }

    template<typename Filter>
    inline bool noneMatch(Filter filter) const { return !anyMatch(filter); }

    inline void clear() {
        nodes.clear();
        checksum++;
    }

    inline unsigned long size() const { return nodes.size(); }

    inline std::vector<Node *> *backing() { return &nodes; }

    inline unsigned long getChecksum() const { return checksum; }

private:
    std::vector<Node *> nodes;
    unsigned long checksum;
};

#endif //FEATHER_NODE_LIST_H
