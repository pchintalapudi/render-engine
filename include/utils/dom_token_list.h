//
// Created by prem on 11/12/2018.
//

#ifndef FEATHER_DOM_TOKEN_LIST_H
#define FEATHER_DOM_TOKEN_LIST_H

#include <algorithm>
#include <vector>
#include "../typedefs.h"

namespace dom {
    class DOMTokenList;
}

class dom::DOMTokenList {
public:
    inline unsigned long getLength() const { return backing.size(); }

    inline DOMString getValue() const {
        DOMString val = cachedsum == checksum ? cached : (cached = _get_cacheable_value());
        cachedsum = checksum;
        return val;
    }

    inline DOMString getItem(unsigned long index) const { return backing[index]; }

    inline bool contains(DOMString val) const {
        return std::find(backing.begin(), backing.end(), val) != backing.end();
    }

    inline void add(DOMString val) {
        backing.push_back(val);
        checksum++;
    }

    inline void add(std::vector<DOMString> vals) {
        backing.insert(backing.end(), vals.begin(), vals.end());
        checksum++;
    }

    void remove(DOMString val);

    void replace(DOMString replacement, DOMString target);

    virtual bool supports(DOMString feature) = 0;

    bool toggle(DOMString token);

    template<typename Op>
    void forEach(Op op) {
        std::for_each(backing.begin(), backing.end(), op);
    }

    inline unsigned long getChecksum() { return checksum; }

    void clear() {
        backing.clear();
        checksum++;
    }

private:
    std::vector<DOMString> backing;
    unsigned long checksum;
    mutable unsigned long cachedsum;
    mutable DOMString cached;

    DOMString _get_cacheable_value() const;
};

#endif //FEATHER_DOM_TOKEN_LIST_H
