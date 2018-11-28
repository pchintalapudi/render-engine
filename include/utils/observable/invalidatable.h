//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_INVALIDATABLE_H
#define FEATHER_INVALIDATABLE_H

#include <functional>
#include <map>

namespace observable {
    class Invalidatable;
}

class observable::Invalidatable {
public:
    inline void addInvalidator(void *key, std::function<void()> invalidator) { invalidators[key] = invalidator; }

    inline void removeInvalidator(void *key) { invalidators.erase(key); }

    virtual ~Invalidatable() {}

protected:
    inline void invalidate() const {
        for (auto it : invalidators)it.second();
        valid = false;
    }

    bool isValid() const { return valid; }

    void validate() const { valid = true; }

private:
    mutable bool valid;
    std::map<void *, std::function<void()>> invalidators;
};

#endif //FEATHER_INVALIDATABLE_H
