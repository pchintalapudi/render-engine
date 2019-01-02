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

            inline StrongPointer<Attr> getNamedItem(const DOMString &name) const { return backing.at(name); }

            inline StrongPointer<Attr> getNamedItemNS(const DOMString &ns, const DOMString &local) const {
                return backing.at(ns + ":" + local);
            }

            void setNamedItem(StrongPointer<Attr> attr);

            inline void setNamedItemNS(StrongPointer<Attr> attr) { setNamedItem(std::move(attr)); }

            StrongPointer<Attr> removeNamedItem(DOMString name);

            inline StrongPointer<Attr> removeNamedItemNS(const DOMString &ns, const DOMString &local) {
                return removeNamedItem(ns + ":" + local);
            }

            inline UInt getLength() const { return backing.size(); }

            inline bool contains(const DOMString &key) const { return backing.find(key) != backing.end(); }

            Vector<DOMString> getKeys() const;

            DOMString toHTML() const;

        protected:
            void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *) const override {
                s -= observable::InvEvent::INVALIDATE_THIS;
            }

        private:
            Map<DOMString, StrongPointer<Attr>> backing = Map<DOMString, StrongPointer<Attr>>();
            List<DOMString> insertionOrder = List<DOMString>();

            void invalidate() {
                observable::Invalidatable::invalidate(
                        RegularEnumSet<observable::InvEvent>() += observable::InvEvent::ATTRIBUTE_SIZE_CHANGE,
                        this);
            }
        };
    }
}
#endif //FEATHER_NAMED_NODE_MAP_H
