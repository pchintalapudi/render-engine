//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_NAMED_NODE_MAP_H
#define FEATHER_NAMED_NODE_MAP_H

#include "observable/invalidatable.h"
#include "attr.h"

namespace feather {
    namespace dom {
        class NamedNodeMap : public observable::Invalidatable {
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

            inline UInt size() const { return backing.size(); }

            inline UInt getLength() const { return size(); }

            inline bool contains(const DOMString &key) const { return backing.find(key) != backing.end(); }

            Vector<DOMString> getKeys() const;

            DOMString toHTML() const;

            bool operator==(const NamedNodeMap &other);

            inline bool operator!=(const NamedNodeMap &other) { return !(*this == other); }

            //Danger zone

            Map<DOMString, StrongPointer<Attr>> *getBacking() { return &backing; }

            const Map<DOMString, StrongPointer<Attr>> *getBacking() const { return &backing; }

            List<DOMString> *getInsertionOrder() { return &insertionOrder; }

            const List<DOMString> *getInsertionOrder() const { return &insertionOrder; }

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
