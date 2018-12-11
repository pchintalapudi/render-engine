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
            UNKNOWN_NODE,
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

            Node(DOMString baseURI, DOMString name, NodeType type, DOMString *const value, StrongPointer<Node> parent)
                    : baseURI(baseURI), name(name), type(type), value(value), parent(WeakPointer<Node>(parent)) {}

            Node(Node &other);

            Node(Node &&other) noexcept;

            Node &operator=(Node other);

            Node &operator=(const Node &other);

            Node &operator=(Node &&other) noexcept;

            inline DOMString getBaseURI() const { return baseURI; }

            inline observable::ObservableList<StrongPointer<Node>> &getChildNodes() { return childNodes; }

            inline StrongPointer<Node> getFirstChild() const {
                return childNodes.empty() ? StrongPointer<Node>(nullptr) : childNodes.get(0);
            }

            inline bool isConnected() const { return getOwnerDocument().get() != nullptr; }

            inline StrongPointer<Node> getLastChild() const {
                return childNodes.empty() ? StrongPointer<Node>(nullptr) : childNodes.get(childNodes.size() - 1);
            }

            //TODO: Implement me
            StrongPointer<Node> getNextSibling();

            inline DOMString getNodeName() const { return name; }

            inline NodeType getNodeType() const { return type; }

            const DOMString *getNodeValue() const { return value; }

            void setNodeValue(DOMString value) { *this->value = value; }

            //TODO: Implement me
            StrongPointer<Document> getOwnerDocument() const;

            inline StrongPointer<Node> getParentNode() const {
                return parent.expired() ? StrongPointer<Node>(nullptr) : parent.lock();
            }

            //TODO: Implement me
            StrongPointer<Element> getParentElement() const;

            //TODO: Implement me
            StrongPointer<Node> getPrevSibling() const;

            //TODO: Implement me
            DOMString getTextContent() const;

            //TODO: Implement me
            void setTextContent(DOMString textContent);

            //TODO: Implement me
            StrongPointer<Node> appendChild(StrongPointer<Node> child);

            virtual StrongPointer<Node> cloneNode() const = 0;

            //TODO: Implement me
            UByte compareDocumentPosition(const Node &other) const;

            //TODO: Implement me
            bool contains(const Node &other) const;

            inline StrongPointer<Node> getRootNode() { return getRootNode(false); }

            //TODO: Implement me
            StrongPointer<Node> getRootNode(bool composed);

            inline bool hasChildNodes() const { return !childNodes.empty(); }

            //TODO: Implement me
            StrongPointer<Node> insertBefore(StrongPointer<Node> add, StrongPointer<Node> ref);

            //TODO: Implement me
            StrongPointer<Node> insertAfter(StrongPointer<Node> add, StrongPointer<Node> ref);

            //TODO: Implement me
            bool isDefaultNamespace(DOMString ns);

            virtual bool isEqualNode(const Node &other) = 0;

            inline bool isSameNode(const Node &other) { return this == &other; }

            //TODO: Implement me
            DOMString lookupPrefix(DOMString ns);

            //TODO: Implement me
            DOMString lookupNamespace(DOMString prefix);

            //TODO: Implement me
            void normalize();

            //TODO: Implement me
            StrongPointer<Node> removeChild(StrongPointer<Node> child);

            //TODO: Implement me
            StrongPointer<Node> replaceChild(StrongPointer<Node> newChild, StrongPointer<Node> oldChild);

        private:
            DOMString baseURI;
            observable::ObservableList<StrongPointer<Node>> childNodes
                    = observable::ObservableList<StrongPointer<Node>>();
            observable::ObservableItem<WeakPointer<Node>> nextSibling = observable::ObservableItem<WeakPointer<Node>>();
            DOMString name;
            NodeType type;
            DOMString *const value;
            WeakPointer<Node> parent;
            observable::ObservableItem<WeakPointer<Node>> prevSibling = observable::ObservableItem<WeakPointer<Node>>();
        };
    }
}
#endif //FEATHER_NODE_H
