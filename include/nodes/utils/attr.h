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

        class Attr : observable::Invalidatable {
        public:

            Attr(DOMString localName, const StrongPointer <Element> &owner) : Attr("", "", std::move(localName),
                                                                                   owner) {}

            Attr(DOMString ns, DOMString prefix, DOMString localName, const StrongPointer <Element> &owner)
                    : name(prefix + localName), ns(std::move(ns)), prefix(std::move(prefix)),
                      localName(std::move(localName)), owner(owner) {}

            inline DOMString getNamespace() const { return ns; }

            inline DOMString getPrefix() const { return prefix; }

            inline DOMString getLocalName() const { return localName; }

            inline DOMString getName() const { return name; }

            inline StrongPointer <Element> getOwner() const { return owner.lock(); }

            inline bool getSpecified() const { return true; }

            inline DOMString getValue() const { return toString(); }

            virtual DOMString toString() const = 0;

            virtual void fromString(DOMString) = 0;

            virtual StrongPointer <Attr> clone(const StrongPointer <Element> &element) const = 0;

            inline void setValue(DOMString value) {
                fromString(std::move(value));
                invalidate();
            }

            bool operator==(const Attr &other) const {
                return getName() == other.getName() && getValue() == other.getValue();
            }

            bool operator!=(const Attr &other) const {
                return !(*this == other);
            }

        private:
            DOMString name;
            DOMString ns;
            DOMString prefix;
            DOMString localName;
            WeakPointer <Element> owner;

            void invalidate() {
                observable::Invalidatable::invalidate(
                        RegularEnumSet<observable::InvEvent>() += observable::InvEvent::ATTRIBUTE_INTERNAL_CHANGE,
                        this);
            }
        };

        class StandardAttr : public Attr {
        public:

            StandardAttr(DOMString ns, DOMString prefix, DOMString localName, const StrongPointer <Element> &owner)
                    : Attr(std::move(ns), std::move(prefix), std::move(localName), owner) {}

            StandardAttr(DOMString localName, const StrongPointer <Element> &owner)
                    : Attr(std::move(localName), owner) {}

            StandardAttr(DOMString localName, const StrongPointer <Element> &owner, DOMString initial)
                    : StandardAttr(std::move(localName), owner) { this->value = std::move(initial); }

            StandardAttr(DOMString ns, DOMString prefix, DOMString localName, const StrongPointer <Element> &owner,
                         DOMString initial)
                    : Attr(std::move(ns), std::move(prefix), std::move(localName), owner) {
                value = std::move(initial);
            }

            inline DOMString toString() const override { return value; }

            inline void fromString(DOMString value) override { this->value = value; }

            StrongPointer <Attr> clone(const StrongPointer <Element> &element) const override {
                return std::make_shared<StandardAttr>(getNamespace(), getPrefix(), getLocalName(), element, value);
            }

        private:
            DOMString value = "";
        };

        class ClassAttr : public Attr {
        public:

            ClassAttr(const StrongPointer <Element> &owner, const StrongPointer <DOMTokenList> &classList)
                    : Attr("", "", "class", owner), classList(classList) {}

            ClassAttr(const StrongPointer <Element> &owner, const StrongPointer <DOMTokenList> &classList,
                      DOMString initial) : ClassAttr(owner, classList) {
                fromString(std::move(initial));
            }

            inline DOMString toString() const override {
                return classList.expired() ? "" : classList.lock()->getValue();
            }

            inline void fromString(DOMString value) override {
                if (!classList.expired())
                    classList.lock()->setValue(value);
            }

            StrongPointer <Attr> clone(const StrongPointer <Element> &element) const override;

        private:
            WeakPointer <DOMTokenList> classList;
        };
    }
}
#endif //FEATHER_ATTR_H
