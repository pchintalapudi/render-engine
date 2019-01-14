//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_DOM_TOKEN_LIST_H
#define FEATHER_DOM_TOKEN_LIST_H

#include "observable/invalidatable.h"

namespace feather {
    namespace dom {
        class DOMTokenList : public observable::Invalidatable {
        public:
            inline UInt getLength() const { return source.size(); }

            const DOMString &getValue() const;

            void setValue(DOMString value);

            inline const DOMString &getItem(UInt idx) const { return source[idx]; }

            void add(DOMString value);

            void remove(const DOMString &value);

            bool toggle(DOMString value);

            bool toggle(DOMString value, bool force);

            bool replace(const DOMString &old, DOMString replace);

            inline bool supports(const DOMString &feature) const {
                return features.get() && features->find(feature) != features->end();
            }

            inline bool contains(const DOMString &str) const {
                return std::find(source.begin(), source.end(), str) != source.end();
            }

        private:
            mutable DOMString cached{};
            Vector<DOMString> source{};
            StrongPointer<Set<DOMString>> features;
        };
    }
}
#endif //FEATHER_DOM_TOKEN_LIST_H
