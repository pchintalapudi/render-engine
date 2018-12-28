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
            ObservableList() : extractor(nullptr) {}

            explicit ObservableList(StrongPointer <Function<StrongPointer<Invalidatable>(E)>> extractor)
                    : extractor(extractor) {}

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
                source.insert(source.begin() + index, e);
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

            inline void clear() {
                source.clear();
                invalidate();
            }

            inline bool empty() const { return source.empty(); }

            inline void reserve(UInt size) { source.reserve(size); }

            inline void swap(Vector <E> v) {
                source.swap(v);
                invalidate();
            }

            E remove(UInt idx) {
                E val = source[idx];
                unbindE(val);
                source.erase(source.begin() + idx);
                invalidate();
                return val;
            }

            inline void removeAll(UInt start, UInt end) {
                WeakPointer<ObservableList<E>> weakThis = std::static_pointer_cast<ObservableList<E>>(
                        shared_from_this());
                std::for_each(source.begin() + start, source.begin() + end,
                              [weakThis](E e) { if (!weakThis.expired()) weakThis.lock()->unbindE(e); });
                source.erase(source.begin() + start, source.begin() + end);
                invalidate();
            }

            ~ObservableList() override = default;

        protected:
            void modify(RegularEnumSet <InvEvent> &s, const Invalidatable *) const override {
                s.remove(InvEvent::INVALIDATED);
            }

        private:

            Vector <E> source = Vector<E>();
            StrongPointer <Function<StrongPointer<Invalidatable>(E)>> extractor;

            inline void bindE(E e) {
                if (extractor.get())
                    (*extractor)(e)->bind(std::static_pointer_cast<ObservableList<E>>(shared_from_this()));
            }

            inline void unbindE(E e) {
                if (extractor.get())
                    (*extractor)(e)->unbind(std::static_pointer_cast<ObservableList<E>>(shared_from_this()));
            }

            inline void invalidate() {
                Invalidatable::invalidate(
                        RegularEnumSet<InvEvent>(1u << static_cast<ULong>(InvEvent::LIST_CHANGE)),
                        this
                );
            }
        };
    }
}
#endif //FEATHER_OBSERVABLE_LIST_H
