//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_ATTR_H
#define FEATHER_ATTR_H

#include "dom_token_list.h"
#include "observable/invalidatable.h"

namespace feather {
    namespace dom {
        class Element;

        template<typename A>
        class Attr : observable::Invalidatable {
        public:

            Attr(DOMString localName, StrongPointer <Element> owner) : Attr("", "", localName, owner) {}

            Attr(DOMString ns, DOMString prefix, DOMString localName, StrongPointer <Element> owner)
                    : ns(ns), prefix(prefix), localName(localName), name(prefix + localName), owner(owner) {}

            inline DOMString getNamespace() { return ns; }

            inline DOMString getPrefix() { return prefix; }

            inline DOMString getLocalName() { return localName; }

            inline DOMString getName() { return name; }

            inline StrongPointer <Element> getOwner() { return owner.expired() ? nullptr : owner.lock(); }

            inline bool getSpecified() { return true; }

            inline DOMString getValue() { return toString(); }

            virtual DOMString toString() const = 0;

            virtual void fromString(DOMString) = 0;

            inline void setValue(DOMString value) {
                fromString(value);
                invalidate();
            }

        private:
            DOMString ns;
            DOMString prefix;
            DOMString localName;
            DOMString name;
            WeakPointer <Element> owner;

            void invalidate() {
                observable::Invalidatable::invalidate(
                        RegularEnumSet<observable::InvEvent>(1u << static_cast<int>(observable::InvEvent::MAP_CHANGE)),
                        this);
            }
        };

        class StandardAttr : public Attr<StandardAttr> {
        public:

            StandardAttr(DOMString ns, DOMString prefix, DOMString localName, StrongPointer <Element> owner)
                    : Attr(ns, prefix, localName, owner) {}

            StandardAttr(DOMString localName, StrongPointer <Element> owner) : Attr(localName, owner) {}

            inline DOMString toString() const override { return value; }

            inline void fromString(DOMString value) override { this->value = value; }

        private:
            DOMString value = "";
        };

        class ClassAttr : public Attr<ClassAttr> {
        public:

            ClassAttr(DOMString ns, DOMString prefix, DOMString localName,
                      StrongPointer <Element> owner, StrongPointer <DOMTokenList> classList)
                    : Attr(ns, prefix, localName, owner), classList(classList) {}

            inline DOMString toString() const override {
                return classList.expired() ? "" : classList.lock()->getValue();
            }

            inline void fromString(DOMString value) override {
                if (!classList.expired())
                    classList.lock()->setValue(value);
            }

        private:
            WeakPointer <DOMTokenList> classList;
        };
    }
}
#endif //FEATHER_ATTR_H
