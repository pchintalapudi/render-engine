//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_DOM_TOKEN_LIST_H
#define FEATHER_DOM_TOKEN_LIST_H

#include <sstream>
#include <algorithm>
#include <string>
#include <vector>

typedef std::string DOMString;

class DOMTokenList {
public:
    inline unsigned long getLength() {
        return this->items.size();
    }

    DOMString getValue() {
        std::stringstream ss;
        std::for_each(this->items.begin(), this->items.end(), [&ss](const std::string &s) { ss << s; });
        return ss.str();
    }

    DOMString item(unsigned long &index) {
        return this->items[index];
    }

    bool contains(DOMString &item) {
        return std::find(this->items.begin(), this->items.end(), item) == this->items.end();
    }

    void add(DOMString item) {
        this->items.push_back(item);
    }

    void remove(DOMString item) {
        auto it = std::find(this->items.begin(), this->items.end(), item);
        if (it != this->items.end()) {
            this->items.erase(it);
        }
    }

    void replace(DOMString newItem, DOMString oldItem) {
        std::replace(this->items.begin(), this->items.end(), oldItem, newItem);
    }

    virtual bool supports(DOMString feature) = 0;

    bool toggle(DOMString item) {
        auto it = std::find(this->items.begin(), this->items.end(), item);
        if (it == this->items.end()) {
            this->items.push_back(item);
            return true;
        } else {
            this->items.erase(it);
            return false;
        }
    }

    inline std::vector<DOMString> getItems() {
        return items;
    }

private:
    std::vector<DOMString> items;
};

#endif //FEATHER_DOM_TOKEN_LIST_H
