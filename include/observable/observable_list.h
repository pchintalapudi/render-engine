//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_OBSERVABLE_LIST_H
#define FEATHER_OBSERVABLE_LIST_H

#include "invalidatable.h"

namespace feather {
    namespace observable {

#define invShort static_cast<Derived *>(this)->invalidate

        template<typename E, typename Derived, StrongPointer<Invalidatable>(*convert)(const E &ref)
        = std::static_pointer_cast<Invalidatable>>
        class ObservableList : public Invalidatable {
        public:
            inline const E &operator[](UInt idx) const { return source[idx]; }

            inline const E &front() const { return source.front(); }

            inline const E &back() const { return source.back(); }

            inline UInt indexOf(const E &element) const {
                auto idx = std::find(source.begin(), source.end(), element) - source.begin();
                return idx < source.size() ? idx : ~0;
            }

            inline bool contains(const E &element) const { return ~indexOf(element) != 0; }

            inline UInt size() const { return source.size(); }

            inline bool empty() const { return source.empty(); }

            inline auto begin() const { return source.begin(); }

            inline auto end() const { return source.end(); }

            inline void reserve(UInt size) { source.reserve(size); }

            void clear() {
                if (convert) for (const auto &e : source) unbindFrom(e);
                source.clear();
                invShort();
            }

            inline void add(E element) {
                if (convert) bindTo(convert(element));
                source.push_back(std::move(element));
                invShort();
            }

            template<typename L>
            inline void addAll(const L &list) {
                reserve(list.size() + size());
                if (convert) {
                    for (const auto &e : list) {
                        bindTo(convert(e));
                        source.push_back(e);
                    }
                } else {
                    source.insert(source.end(), list.begin(), list.end());
                }
                invShort();
            }

            template<typename L>
            void addAll(L &&list) {
                reserve(list.size() + size());
                if (convert) {
                    for (auto &e : list) {
                        bindTo(convert(e));
                        source.push_back(std::move(e));
                    }
                } else {
                    source.insert(source.end(), std::make_move_iterator(list.begin()),
                                  std::make_move_iterator(list.end()));
                }
                invShort();
            }

            void insert(UInt idx, E element) {
                if (convert) bindTo(convert(element));
                source.insert(source.begin() + idx, element);
                invShort();
            }

            template<typename L>
            void insertAll(UInt idx, const L &list) {
                source.reserve(list.size() + size());
                if (convert) for (const auto &e : list) bindTo(convert(e));
                source.insert(source.begin() + idx, list.begin(), list.end());
                invShort();
            }

            template<typename L>
            void insertAll(UInt idx, L &&list) {
                source.reserve(list.size() + size());
                if (convert) for (const auto &e : list) bindTo(convert(e));
                source.insert(source.begin() + idx, std::make_move_iterator(list.begin()),
                              std::make_move_iterator(list.end()));
                invShort();
            }

            void set(UInt idx, E element) {
                if (convert) {
                    unbindFrom(convert(source[idx]));
                    bindTo(convert(element));
                }
                source[idx] = std::move(element);
                invShort();
            }

            void remove(UInt idx) {
                if (convert) unbindFrom(convert(source[idx]));
                source.erase(source.begin() + idx);
                invShort();
            }

            void removeAll(UInt start, UInt finish) {
                if (convert) for (UInt i = start; i < finish; unbindFrom(convert(source[i++])));
                source.erase(source.begin() + start, source.begin() + finish);
                invShort();
            }

        protected:

            void modify(RegularEnumSet <InvEvent> &s,
                        const Invalidatable *) const override { s -= InvEvent::INVALIDATE_THIS; }

        private:
            Vector <E> source{};
        };

#undef invShort

        template<typename Derived, typename ElementType>
        class ListWrapper : public Invalidatable {
        public:

            inline const ElementType &operator[](UInt idx) const { return getVector()[idx]; }

            inline bool contains(const ElementType &et) const {
                return std::find(getVector().begin(), getVector().end(), et) != getVector().end();
            }

            inline UInt size() const { return getVector().size(); }

            inline bool empty() const { return getVector().size() == 0; }

            inline auto begin() const { return getVector().begin(); }

            inline auto end() const { return getVector().end(); }

            inline const ElementType &front() const { return getVector().front(); }

            inline const ElementType &back() const { return getVector().back(); }

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
                if (watched) this->bindTo(watched);
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
