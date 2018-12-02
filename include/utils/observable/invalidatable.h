//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_INVALIDATABLE_H
#define FEATHER_INVALIDATABLE_H

#include <functional>
#include <map>
#include <set>

namespace observable {
    class Invalidatable;
}

class observable::Invalidatable {
public:
    inline void addInvalidator(void *key, std::function<void(long long)> invalidator) {
        invalidators[key] = invalidator;
    }

    inline void removeInvalidator(void *key) { invalidators.erase(key); }

    bool isValid() const { return valid; }

    virtual ~Invalidatable() {}

protected:
    inline void invalidate(long long l) const {
        if (handle(l)) valid = false;
        for (auto it : invalidators)it.second(edit(l));
    }

    virtual bool handle(long long) const {
        return true;
    }

    virtual long long edit(long long l) const { return l; }

    void validate() const { valid = true; }

    const std::function<void(long long)> invalidator = [this](long long l) { this->invalidate(l); };

private:
    mutable bool valid;
    std::map<void *, std::function<void(long long)>> invalidators;
};

#endif //FEATHER_INVALIDATABLE_H
