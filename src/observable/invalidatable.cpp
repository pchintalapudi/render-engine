//
// Created by prem on 12/10/2018.
//

#include "observable/invalidatable.h"

using namespace feather::observable;

void Invalidatable::invalidate(RegularEnumSet <InvEvent> s) const {
    modify(s);
    if (s.contains(InvEvent::PROPAGATE)) {
        for (auto it = dependents.begin(); it != dependents.end();) {
            if (it->expired()) it = dependents.erase(it);
            else it->lock()->invalidate(s);
        }
    }
    lastInvalidationCall = s;
    valid &= s.contains(InvEvent::INVALIDATED);
}

void Invalidatable::bind(feather::WeakPointer<feather::observable::Invalidatable> dependent) const {
    if (!dependent.expired()) {
        auto ptr = dependent.lock();
        if (!valid && ptr->isValid()) ptr->invalidate(lastInvalidationCall);
        dependents.insert(dependent);
    }
}

void Invalidatable::unbind(feather::WeakPointer<feather::observable::Invalidatable> dependent) const {
    if (!dependent.expired()) {
        auto ptrLock = dependent.lock();
        for (auto it = dependents.begin(); it != dependents.end();) {
            if (it->expired()) it = dependents.erase(it);
            else if (it->lock() == ptrLock) {
                dependents.erase(it);
                return;
            }
        }
    }
}

void Invalidatable::gc(feather::UByte) {
    for (auto it = dependents.begin(); it != dependents.end();) {
        if (it->expired()) it = dependents.erase(it);
        else it++;
    }
}