//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_OBSERVABLE_LIST_H
#define FEATHER_OBSERVABLE_LIST_H

#include "invalidatable.h"

namespace feather {
    namespace observable {
        template<typename E, typename Derived>
        class ObservableList : public Invalidatable {
        public:
            ObservableList() : extractor(
                    feather::StrongPointer<feather::Function<StrongPointer<Invalidatable>(E) >>()) {}

            explicit ObservableList(StrongPointer <Function<StrongPointer<Invalidatable>(E)>> extractor)
                    : extractor(std::move(extractor)) {}

            inline const E &get(UInt index) const { return source[index]; }

            bool contains(const E &val) const {
                return std::find(source.begin(), source.end(), val);
            }

            inline UInt size() const { return source.size(); }

            void set(UInt index, E e) {
                unbindE(source[index]);
                bindE(e);
                source[index] = std::move(e);
                invalidate();
            }

            void add(E e) {
                bindE(e);
                source.push_back(std::move(e));
                invalidate();
            }

            template<typename L>
            void addAll(const L &vals) {
                source.reserve(size() + vals.size());
                for (const E &e : vals) {
                    bindE(e);
                    source.push_back(e);
                }
                invalidate();
            }

            void insert(UInt index, E e) {
                bindE(e);
                source.insert(source.begin() + index, std::move(e));
                invalidate();
            }

            template<typename L>
            void insertAll(UInt index, const L &list) {
                std::vector<E> temp;
                temp.reserve(list.size() + size());
                UInt count = 0;
                for (const auto &val : source) {
                    if (count++ == index) {
                        for (auto e : list) {
                            bindE(e);
                            temp.push_back(std::move(e));
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

            E remove(UInt idx) {
                E val = std::move(source[idx]);
                unbindE(val);
                source.erase(source.begin() + idx);
                invalidate();
                return val;
            }

            inline void removeAll(UInt start, UInt end) {
                WeakPointer<ObservableList<E, Derived>> weakThis
                        = std::static_pointer_cast<ObservableList<E, Derived>>(shared_from_this());
                std::for_each(source.begin() + start, source.begin() + end,
                              [weakThis](const E &e) { if (!weakThis.expired()) weakThis.lock()->unbindE(e); });
                source.erase(source.begin() + start, source.begin() + end);
                invalidate();
            }

            ~ObservableList() override = default;

            inline auto begin() const { return source.begin(); }

            inline auto end() const { return source.end(); }

        protected:
            void modify(RegularEnumSet <InvEvent> &s, const Invalidatable *) const override {
                s -= InvEvent::INVALIDATE_THIS;
            }

            using Invalidatable::invalidate;

        private:

            Vector <E> source{};
            StrongPointer <Function<StrongPointer<Invalidatable>(E)>> extractor{};

            inline void bindE(const E &e) {
                if (extractor)
                    (*extractor)(e)->bind(std::static_pointer_cast<ObservableList<E, Derived>>(shared_from_this()));
            }

            inline void unbindE(const E &e) {
                if (extractor)
                    (*extractor)(e)->unbind(std::static_pointer_cast<ObservableList<E, Derived>>(shared_from_this()));
            }

            void invalidate() const {
                static_cast<const Derived *>(this)->invShort();
            }
        };

        template<typename Derived, typename ElementType>
        class ListWrapper : public Invalidatable {
        public:
            inline const ElementType &get(UInt idx) const { return getVector()[idx]; }

            inline bool contains(const ElementType &et) const {
                return std::find(getVector().begin(), getVector().end(), et) != getVector().end();
            }

            inline UInt size() const { return getVector().size(); }

            inline bool empty() const { return getVector().size() == 0; }

            inline auto begin() const { return getVector().begin(); }

            inline auto end() const { return getVector().end(); }

        private:
            const Vector <ElementType> &getVector() const { return static_cast<const Derived *>(this)->getSource(); }
        };

        template<typename ListElementType, class ListType, typename MapType,
                Pair<bool, MapType>(*Adjuster)(const ListElementType &)>
        class SketchyObservableListWrapper
                : public ListWrapper<SketchyObservableListWrapper<ListElementType, ListType, MapType, Adjuster>,
                        MapType> {
        public:
            explicit SketchyObservableListWrapper(StrongPointer<const ListType> watched)
                    : watched(std::move(watched)) {
                if (watched) watched->bind(std::static_pointer_cast<Invalidatable>(this->shared_from_this()));
            }

        private:
            WeakPointer<const ListType> watched;

            mutable Vector <MapType> cached{};

            void recompute() const {
                cached.clear();
                auto ptr = watched.lock();
                if (ptr) {
                    for (const auto &ref : *ptr) {
                        auto p = Adjuster(ref);
                        if (p.first) cached.push_back(p.second);
                    }
                }
            }

            inline const Vector <MapType> &getSource() const {
                if (!this->isValid()) {
                    recompute();
                    this->validate();
                }
                return cached;
            }

            friend class ListWrapper<SketchyObservableListWrapper<ListElementType,
                    ListType, MapType, Adjuster>, MapType>;
        };

        template<typename ElementType, typename Inv, typename Op>
        class RiskyFilteredList : public ListWrapper<RiskyFilteredList<ElementType, Inv, Op>, ElementType> {
        public:
            RiskyFilteredList(StrongPointer<const Inv> watched, Op op)
                    : watched(std::move(watched)), op(std::move(op)) {}

        private:
            WeakPointer<const Inv> watched;
            Op op;

            mutable Vector <ElementType> cached{};

            const Vector <ElementType> &getSource() const {
                if (this->isValid()) return cached;
                cached.clear();
                auto ptr = watched.lock();
                if (ptr) op(ptr, cached);
                return cached;
            }

            friend class ListWrapper<RiskyFilteredList<ElementType, Inv, Op>, ElementType>;
        };
    }
}
#endif //FEATHER_OBSERVABLE_LIST_H
