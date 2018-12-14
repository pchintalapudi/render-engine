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

        class Document;

        class Element;

        class Node : public js::EventTarget {
        public:

            Node(DOMString baseURI, DOMString name, NodeType type,
                 StrongPointer<DOMString> value, StrongPointer<Node> parent);

            inline DOMString getBaseURI() const { return baseURI; }

            inline observable::ObservableList<StrongPointer<Node>> &getChildNodes() { return childNodes; }

            inline const observable::ObservableList<StrongPointer<Node>> &getChildNodes() const { return childNodes; }

            inline StrongPointer<Node> getFirstChild() const {
                return childNodes.empty() ? StrongPointer<Node>(nullptr) : childNodes.get(0);
            }

            inline bool isConnected() const { return getOwnerDocument().get() != nullptr; }

            inline StrongPointer<Node> getLastChild() const {
                return childNodes.empty() ? StrongPointer<Node>(nullptr) : childNodes.get(childNodes.size() - 1);
            }

            StrongPointer<Node> getNextSibling() const;

            inline DOMString getNodeName() const { return name; }

            inline NodeType __MANGLE__getNodeType() const {
                return static_cast<UInt>(type) ? type : NodeType::DOCUMENT_FRAGMENT_NODE;
            }

            inline NodeType getNodeTypeInternal() const { return type; }

            inline StrongPointer<DOMString> getNodeValue() const {
                return value.get() ? StrongPointer<DOMString>(new DOMString(*value)) : nullptr;
            }

            void setNodeValue(DOMString value) { if (this->value) *this->value = value; }

            StrongPointer<Document> getOwnerDocument() const;

            inline StrongPointer<Node> getParentNode() const {
                return parent.get().expired() ? StrongPointer<Node>(nullptr) : parent.get().lock();
            }

            void setParentNode(StrongPointer<Node> parentNode);

            StrongPointer<Element> getParentElement() const;

            StrongPointer<Node> getPrevSibling() const;

            StrongPointer<DOMString> getTextContent() const;

            void setTextContent(DOMString textContent);

            StrongPointer<Node> appendChild(StrongPointer<Node> child);

            inline StrongPointer<Node> cloneNode() { return cloneNode(false); }

            virtual StrongPointer<Node> cloneNode(bool deep) const = 0;

            UByte compareDocumentPosition(StrongPointer<const Node>) const;

            bool contains(StrongPointer<const Node>) const;

            inline StrongPointer<Node> getRootNode() const { return getRootNode(false); }

            StrongPointer<Node> getRootNode(bool composed) const;

            inline bool hasChildNodes() const { return !childNodes.empty(); }

            StrongPointer<Node> insertBefore(StrongPointer<Node> add, StrongPointer<const Node> ref);

            StrongPointer<Node> insertAfter(StrongPointer<Node> add, StrongPointer<const Node> ref);

            //TODO: Implement me
            bool isDefaultNamespace(DOMString ns) const;

            virtual bool isEqualNode(const Node &other) const = 0;

            inline bool isSameNode(const Node &other) { return this == &other; }

            //TODO: Implement me
            DOMString lookupPrefix(DOMString ns) const;

            //TODO: Implement me
            DOMString lookupNamespace(DOMString prefix) const;

            void normalize();

            StrongPointer<Node> removeChild(StrongPointer<Node> child);

            StrongPointer<Node> replaceChild(StrongPointer<Node> newChild, StrongPointer<Node> oldChild);

            //Internal speedier methods for various interfaces

            //From ChildNode

            void insertBeforeChild(Vector<StrongPointer<Node>>, StrongPointer<const Node> ref);

            void insertAfterChild(Vector<StrongPointer<Node>>, StrongPointer<const Node> ref);

            void replaceChild(Vector<StrongPointer<Node>>, StrongPointer<const Node> replace);

            //From NonDocumentTypeChildNode

            StrongPointer<Element> getElementAfterChild(StrongPointer<const Node> ref) const;

            StrongPointer<Element> getElementBeforeChild(StrongPointer<const Node> ref) const;

            inline void bindOwner(StrongPointer<observable::WatchedObservableItem<WeakPointer<Document>>> other) {
                ownerPtr->bind(other);
            }

            inline void unbindOwner(StrongPointer<observable::WatchedObservableItem<WeakPointer<Document>>> other) {
                ownerPtr->unbind(other);
            }

            inline StrongPointer<observable::WatchedObservableItem<WeakPointer<Document>>> getOwnerItem() {
                return ownerPtr;
            };

        protected:
            inline StrongPointer<DOMString> getValuePointer() const { return value; }

            void modify(RegularEnumSet<observable::InvEvent> &, const Invalidatable *) const override;

        private:
            DOMString baseURI;
            observable::ObservableList<StrongPointer<Node>> childNodes;
            StrongPointer<observable::WatchedObservableItem<WeakPointer<Node>>> nextSiblingPtr, prevSiblingPtr;
            DOMString name;
            NodeType type;
            StrongPointer<DOMString> value;
            observable::SourceObservableItem<WeakPointer<Node>> parent
                    = observable::SourceObservableItem<WeakPointer<Node>>();
            StrongPointer<observable::WatchedObservableItem<WeakPointer<Document>>> ownerPtr;
        };
    }
}
#endif //FEATHER_NODE_H
