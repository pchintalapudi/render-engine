//
// Created by prem on 12/10/2018.
//

#include "observable/invalidatable.h"

using namespace feather::observable;

void Invalidatable::invalidate(RegularEnumSet <InvEvent> s, const Invalidatable *i) const {
    modify(s, i);
    if (!s.contains(InvEvent::STOP_PROPAGATION)) {
        for (auto it = dependents.begin(); it != dependents.end();) {
            if (it->expired()) it = dependents.erase(it); else it++->lock()->invalidate(s, this);
        }
    }
    valid &= s.contains(InvEvent::INVALIDATE_THIS);
}

void Invalidatable::bind(feather::StrongPointer<const feather::observable::Invalidatable> dependent) const {
    dependent->invalidate(RegularEnumSet<InvEvent>() + InvEvent::REBOUND, this);
    dependents.insert(dependent);
}

void Invalidatable::unbind(feather::StrongPointer<const feather::observable::Invalidatable> dependent) const {
    for (auto it = dependents.begin(); it != dependents.end();) {
        if (it->expired()) it = dependents.erase(it);
        else if (it->lock() == dependent) {
            dependents.erase(it);
            return;
        } else ++it;
    }
}

void Invalidatable::gc(unsigned char) {
    for (auto it = dependents.begin(); it != dependents.end();) {
        if (it->expired()) it = dependents.erase(it);
        else it++;
    }
}