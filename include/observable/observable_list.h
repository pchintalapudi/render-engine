//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_OBSERVABLE_LIST_H
#define FEATHER_OBSERVABLE_LIST_H

#include "invalidatable.h"

namespace feather {
    namespace observable {
        template<typename E>
        class ObservableList : public Invalidatable {
        public:
            ObservableList() : extractor(nullExtractor) {}

            explicit ObservableList(StrongPointer<Function<Invalidatable(E)>> extractor) : extractor(extractor) {}

            inline E get(UInt index) const { return source[index]; }

            bool contains(E val) const {
                for (E e : source) if (e == val) return true;
                return false;
            }

            inline UInt size() const { return source.size(); }

            void set(UInt index, E e) {
                unbindE(source[index]);
                bindE(e);
                source[index] = e;
                invalidate();
            }

            void add(E e) {
                bindE(e);
                source.push_back(e);
                invalidate();
            }

            void addAll(std::vector<E> vals) {
                source.reserve(size() + vals.size());
                for (E e : vals) {
                    bindE(e);
                    source.push_back(e);
                }
                invalidate();
            }

            template<typename L>
            void addAll(L vals) {
                source.reserve(size() + vals.size());
                for (UInt i = 0; i < vals.size(); i++) {
                    E e = vals.get(i);
                    bindE(e);
                    source.push_back(e);
                }
                invalidate();
            }

            void insert(UInt index, E e) {
                bindE(e);
                source.insert(source.begin() + index);
                invalidate();
            }

            void insertAll(UInt index, std::vector<E> vec) {
                std::vector<E> temp;
                temp.reserve(vec.size() + size());
                UInt count = 0;
                for (auto val : source) {
                    if (count++ == index) {
                        for (auto e : vec) {
                            bindE(e);
                            temp.push_back(e);
                        }
                    }
                    temp.push_back(val);
                }
                source.swap(temp);
                invalidate();
            }

            template<typename L>
            void insertAll(UInt index, L list) {
                std::vector<E> temp;
                temp.reserve(list.size() + size());
                UInt count = 0;
                for (auto val : source) {
                    if (count++ == index) {
                        for (UInt i = 0; i < list.size(); i++) {
                            auto e = list.get(i);
                            bindE(e);
                            temp.push_back(e);
                        }
                    }
                    temp.push_back(val);
                }
                source.swap(temp);
                invalidate();
            }

            ~ObservableList() override = default;

        private:

            static StrongPointer<Function<Invalidatable(E)>> nullExtractor
                    = StrongPointer<Function<Invalidatable(E)>>(nullptr);

            Vector<E> source = Vector<E>();
            StrongPointer<Function<Invalidatable(E)>> extractor;

            inline void bindE(E e) { if (extractor.get())(*extractor)(e).bind(std::make_shared(this)); }

            inline void unbindE(E e) { if (extractor.get())(*extractor)(e).unbind(std::make_shared(this)); }

            inline void invalidate() { invalidate(set(1, InvEvent::LIST_CHANGE)); }
        };
    }
}
#endif //FEATHER_OBSERVABLE_LIST_H
