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
            StrongPointer<Attr> getItem(UInt idx) const;

            inline StrongPointer<Attr> getNamedItem(DOMString name) const { return backing.at(name); }

            inline StrongPointer<Attr> getNamedItemNS(DOMString ns, DOMString local) const {
                return backing.at(ns + ":" + local);
            }

            void setNamedItem(StrongPointer<Attr> attr);

            inline void setNamedItemNS(StrongPointer<Attr> attr) { setNamedItem(attr); }

            StrongPointer<Attr> removeNamedItem(DOMString name);

            inline StrongPointer<Attr> removeNamedItemNS(DOMString ns, DOMString local) {
                return removeNamedItem(ns + ":" + local);
            }

            inline UInt getLength() const { return backing.size(); }

            inline bool contains(DOMString key) const { return backing.find(key) != backing.end(); }

            Vector<DOMString> getKeys() const;

            DOMString toHTML() const;

        protected:
            void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *) {
                s.remove(observable::InvEvent::INVALIDATED);
            }

        private:
            Map<DOMString, StrongPointer<Attr>> backing = Map<DOMString, StrongPointer<Attr>>();
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
