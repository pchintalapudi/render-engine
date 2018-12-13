//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_NAMED_NODE_MAP_H
#define FEATHER_NAMED_NODE_MAP_H

#include "observable/invalidatable.h"
#include "attr.h"

namespace feather {
    namespace dom {
        class NamedNodeMap : observable::Invalidatable {
        public:
            StrongPointer<Attr<void>> getItem(UInt idx) const;

            inline StrongPointer<Attr<void>> getNamedItem(DOMString name) const { return backing.at(name); }

            inline StrongPointer<Attr<void>> getNamedItemNS(DOMString ns, DOMString local) const {
                return backing.at(ns + ":" + local);
            }

            void setNamedItem(StrongPointer<Attr<void>> attr);

            inline void setNamedItemNS(StrongPointer<Attr<void>> attr) { setNamedItem(attr); }

            StrongPointer<Attr<void>> removeNamedItem(DOMString name);

            inline StrongPointer<Attr<void>> removeNamedItemNS(DOMString ns, DOMString local) {
                return removeNamedItem(ns + ":" + local);
            }

            inline UInt getLength() const { return backing.size(); }

        protected:
            void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *) {
                s.remove(observable::InvEvent::INVALIDATED);
            }

        private:
            Map<DOMString, StrongPointer<Attr<void>>> backing = Map<DOMString, StrongPointer<Attr<void>>>();
            List<DOMString> insertionOrder = List<DOMString>();

            void invalidate() {
                observable::Invalidatable::invalidate(
                        RegularEnumSet<observable::InvEvent>(1u << static_cast<int>(observable::InvEvent::MAP_CHANGE)),
                        this);
            }
        };
    }
}
#endif //FEATHER_NAMED_NODE_MAP_H
