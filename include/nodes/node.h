//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_NODE_H
#define FEATHER_NODE_H

#include "events/event_target.h"
#include "observable/observable_list.h"
#include "observable/observable_item.h"

namespace feather {
    namespace dom {

        enum class NodeType {
            SHADOW_ROOT,//Nonstandard
            ELEMENT_NODE,
            ATTRIBUTE_NODE,
            TEXT_NODE,
            CDATA_SECTION_NODE,
            ENTITY_REFERENCE_NODE,
            ENTITY_NODE,
            PROCESSING_INSTRUCTION_NODE,
            COMMENT_NODE,
            DOCUMENT_NODE,
            DOCUMENT_TYPE_NODE,
            DOCUMENT_FRAGMENT_NODE,
            NOTATION_NODE
        };

        class Node;

        class NodeList : public observable::ObservableList<StrongPointer<Node>, NodeList> {
        public:

            NodeList() = default;

            void invalidate() const;

            bool deepEquals(const NodeList &other) const;

            inline bool operator==(const NodeList &other) const { return deepEquals(other); }

            ~NodeList() override = default;

        protected:
            feather::EnumSet<observable::InvEvent>
            modify(EnumSet<observable::InvEvent> s, const Invalidatable *p) const override;
        };

        class Document;

        class Element;

        StrongPointer<observable::Invalidatable> toInvalidatable(const WeakPointer<Node> &ref);

        StrongPointer<observable::Invalidatable> toInvalidatable(const WeakPointer<Document> &ref);

        class Node : public js::EventTarget {
        public:

            Node(DOMString baseURI, DOMString name, NodeType type,
                 StrongPointer<DOMString> value, const StrongPointer<Node> &parent);

            inline const DOMString &getBaseURI() const { return baseURI; }

            inline StrongPointer<NodeList> getChildNodes() {
                return StrongPointer<NodeList>(shared_from_this(), &childNodes);
            }

            inline StrongPointer<const NodeList> getChildNodes() const {
                return StrongPointer<const NodeList>(shared_from_this(), &childNodes);
            }

            inline StrongPointer<Node> getFirstChild() const {
                return childNodes.empty() ? StrongPointer<Node>() : childNodes[0];
            }

            inline bool isConnected() const { return getOwnerDocument().get() != nullptr; }

            inline StrongPointer<Node> getLastChild() const {
                return childNodes.empty() ? StrongPointer<Node>() : childNodes[childNodes.size() - 1];
            }

            StrongPointer<Node> getNextSibling() const;

            inline const DOMString &getNodeName() const { return name; }

            inline NodeType __MANGLE__getNodeType() const {
                return static_cast<UInt>(type) ? type : NodeType::DOCUMENT_FRAGMENT_NODE;
            }

            inline NodeType getNodeTypeInternal() const { return type; }

            inline StrongPointer<const DOMString> getNodeValue() const {
                return value;
            }

            inline const DOMString &getNodeValueUnsafe() const { return *value; }

            inline void setNodeValue(DOMString value) { if (this->value) setNodeValueUnsafe(std::move(value)); }

            inline void setNodeValueUnsafe(DOMString value) { *this->value = std::move(value); }

            StrongPointer<Document> getOwnerDocument() const;

            inline StrongPointer<Node> getParentNode() const {
                return parent->expired() ? StrongPointer<Node>() : parent->lock();
            }

            void setParentNode(const StrongPointer<Node> &parentNode);

            inline void clearParentNode() { setParentNode(StrongPointer<Node>()); }

            StrongPointer<Element> getParentElement() const;

            StrongPointer<Node> getPrevSibling() const;

            StrongPointer<const DOMString> getTextContent() const;

            void setTextContent(DOMString textContent);

            StrongPointer<Node> appendChild(StrongPointer<Node> child);

            inline StrongPointer<Node> cloneNode() { return cloneNode(false); }

            virtual StrongPointer<Node> cloneNode(bool deep) const = 0;

            unsigned char compareDocumentPosition(const StrongPointer<const Node> &other) const;

            bool contains(const StrongPointer<const Node> &other) const;

            inline StrongPointer<Node> getRootNode() const { return getRootNode(false); }

            StrongPointer<Node> getRootNode(bool composed) const;

            inline bool hasChildNodes() const { return !childNodes.empty(); }

            StrongPointer<Node> insertBefore(StrongPointer<Node> add, StrongPointer<const Node> ref);

            StrongPointer<Node> insertAfter(StrongPointer<Node> add, StrongPointer<const Node> ref);

            //TODO: Implement me
            bool isDefaultNamespace(DOMString ns) const;

            virtual bool isEqualNode(const StrongPointer<const Node> &other) const = 0;

            inline bool operator==(const Node &other) const { return isEqualNode(other.getSharedFromThis()); }

            inline bool operator!=(const Node &other) const { return !(*this == other); }

            inline bool isSameNode(const StrongPointer<Node> &other) { return this == other.get(); }

            //TODO: Implement me
            DOMString lookupPrefix(DOMString ns) const;

            //TODO: Implement me
            DOMString lookupNamespace(DOMString prefix) const;

            void normalize();

            StrongPointer<Node> removeChild(StrongPointer<Node> child);

            StrongPointer<Node> replaceChild(StrongPointer<Node> newChild, StrongPointer<Node> oldChild);

            UInt getIndex() const;

            void updateChildIndeces() const;

            inline StrongPointer<Node> getSharedFromThis() {
                return std::static_pointer_cast<Node>(shared_from_this());
            }

            inline StrongPointer<const Node> getSharedFromThis() const {
                return std::static_pointer_cast<const Node>(shared_from_this());
            }

            void insertBeforeChildNDTCN(const StrongPointer<const Node> &ref, const Vector<StrongPointer<Node>> &add);

            void insertAfterChildNDTCN(const StrongPointer<const Node> &ref, const Vector<StrongPointer<Node>> &add);

            void replaceChildNDTCN(const StrongPointer<const Node> &ref, const Vector<StrongPointer<Node>> &add);

        protected:
            inline StrongPointer<DOMString> getValuePointer() const { return value; }

            EnumSet<observable::InvEvent>
            modify(EnumSet<observable::InvEvent> types, const Invalidatable *source) const override;

        private:
            DOMString baseURI{};
            NodeList childNodes{};
            DOMString name{};
            NodeType type;
            StrongPointer<DOMString> value{};
            observable::ObservableItem<WeakPointer<Node>, true, toInvalidatable> parent{};
            mutable observable::ObservableItem<UInt, false> nodeIndex{0};
            mutable observable::ObservableItem<WeakPointer<Document>, false, toInvalidatable> owner{};

            DOMString getTextContentInternal() const;
        };
    }
}
#endif //FEATHER_NODE_H
