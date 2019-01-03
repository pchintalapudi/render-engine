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
            ObservableList() : extractor(StrongPointer < Function < StrongPointer<Invalidatable>(E) >> ()) {}

            explicit ObservableList(StrongPointer <Function<StrongPointer<Invalidatable>(E)>> extractor)
                    : extractor(std::move(extractor)) {}

            inline E get(UInt index) const { return source[index]; }

            bool contains(const E &val) const {
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

            void addAll(const std::vector<E> &vals) {
                source.reserve(size() + vals.size());
                for (E e : vals) {
                    bindE(e);
                    source.push_back(e);
                }
                invalidate();
            }

            template<typename L>
            void addAll(const L &vals) {
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

            void insertAll(UInt index, const std::vector<E> &vec) {
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
            void insertAll(UInt index, const L &list) {
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

            inline void swap(const Vector <E> &v) {
                if (extractor) for (auto e : source) unbindE(e);
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
                WeakPointer<ObservableList<E, Derived>> weakThis
                        = std::static_pointer_cast<ObservableList<E, Derived>>(shared_from_this());
                std::for_each(source.begin() + start, source.begin() + end,
                              [weakThis](E e) { if (!weakThis.expired()) weakThis.lock()->unbindE(e); });
                source.erase(source.begin() + start, source.begin() + end);
                invalidate();
            }

            ~ObservableList() override = default;

        protected:
            void modify(RegularEnumSet <InvEvent> &s, const Invalidatable *) const override {
                s -= InvEvent::INVALIDATE_THIS;
            }

        private:

            Vector <E> source = Vector<E>();
            StrongPointer <Function<StrongPointer<Invalidatable>(E)>> extractor;

            inline void bindE(E e) {
                if (extractor)
                    (*extractor)(e)->bind(std::static_pointer_cast<ObservableList<E, Derived>>(shared_from_this()));
            }

            inline void unbindE(E e) {
                if (extractor)
                    (*extractor)(e)->unbind(std::static_pointer_cast<ObservableList<E, Derived>>(shared_from_this()));
            }

            void invalidate() const {
                static_cast<const Derived *>(this)->invalidate();
            }
        };

        template<typename ListElementType, class ListType, typename MapType,
                Pair<bool, MapType>(*Adjuster)(const ListElementType &)>
        class SketchyObservableListWrapper : public Invalidatable {
        public:
            explicit SketchyObservableListWrapper(StrongPointer<const ListType> watched)
                    : watched(std::move(watched)) {
                if (watched) watched->bind(std::static_pointer_cast<Invalidatable>(shared_from_this()));
            }

            inline MapType get(UInt idx) const { return getVector()[idx]; }

            inline bool contains(MapType mt) const {
                return std::find(getVector().begin(), getVector().end(), std::move(mt)) != getVector().end();
            }

            inline UInt size() const { return getVector().size(); }

            inline bool empty() const { return getVector().empty(); }

        private:
            WeakPointer<const ListType> watched;

            mutable Vector <MapType> cached;

            void recompute() const {
                cached.clear();
                auto ptr = watched.lock();
                if (ptr) {
                    for (UInt i = 0; i < ptr->size(); i++) {
                        Pair<bool, MapType> p = Adjuster(ptr->get(i));
                        if (p.first) cached.push_back(p.second);
                    }
                }
            }

            inline Vector <MapType> &getVector() const {
                if (!isValid()) recompute();
                return cached;
            }
        };

        template<typename ListType, typename MapType, typename AdjusterType>
        class LessSketchyObservableListWrapper : public Invalidatable {
        public:
            explicit LessSketchyObservableListWrapper(StrongPointer<const ListType> watched, AdjusterType adjuster)
                    : watched(std::move(watched)), adjuster(std::move(adjuster)) {}

            inline MapType get(UInt idx) const { return getVector()[idx]; }

            inline bool contains(MapType mt) const {
                return std::find(getVector().begin(), getVector().end(), std::move(mt)) != getVector().end();
            }

            inline UInt size() const { return getVector().size(); }

            inline bool empty() const { return getVector().empty(); }

        private:
            WeakPointer<const ListType> watched;
            AdjusterType adjuster;

            mutable Vector <MapType> cached;

            void recompute() const {
                cached.clear();
                auto ptr = watched.lock();
                if (ptr) {
                    for (UInt i = 0; i < ptr->size(); i++) {
                        Pair<bool, MapType> p = adjuster(ptr->get(i));
                        if (p.first) cached.push_back(p.second);
                    }
                }
            }

            inline Vector <MapType> &getVector() const {
                if (!isValid()) recompute();
                return cached;
            }
        };

        template<typename ListType, typename Filter>
        using FilteredList = LessSketchyObservableListWrapper<ListType, ListType, Filter>;

        template<typename ElementType, typename Inv, typename Op>
        class RiskyFilteredList : Invalidatable {
        public:
            explicit RiskyFilteredList(StrongPointer<const Inv> watched, Op op)
                    : watched(std::move(watched)), op(std::move(op)) {}

            inline ElementType get(UInt i) const { return getVector()[i]; }

            inline UInt size() const { return getVector().size(); }

            inline bool contains(const ElementType &e) const {
                return std::find(getVector().begin(), getVector().end(), e);
            }

            inline bool empty() const { return getVector().empty(); }

        private:
            WeakPointer<const Inv> watched;
            Op op;

            mutable Vector <ElementType> cached;

            Vector <ElementType> &getVector() {
                if (isValid()) return cached;
                else {
                    cached.clear();
                    auto ptr = watched.lock();
                    if (ptr) op(ptr, cached);
                    return cached;
                }
            }
        };
    }
}
#endif //FEATHER_OBSERVABLE_LIST_H
