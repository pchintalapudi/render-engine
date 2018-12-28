//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_CHARACTER_DATA_H
#define FEATHER_CHARACTER_DATA_H

#include "../node.h"
#include "../elements/element.h"

namespace feather {
    namespace dom {
        class CharacterData : public Node {
        public:

            CharacterData(DOMString baseURI, DOMString name, NodeType type, const StrongPointer<Node> &parent)
                    : Node(std::move(baseURI), std::move(name), type, std::make_shared<DOMString>(), parent) {}

            CharacterData(DOMString baseURI, DOMString name, NodeType type, const StrongPointer<Node> &parent,
                          DOMString initText)
                    : Node(std::move(baseURI), std::move(name), type, std::make_shared<DOMString>(), parent) {
                setData(std::move(initText));
            }

            DOMString getData() const { return *getValuePointer(); }

            inline void setData(DOMString data) { *getValuePointer() = std::move(data); }

            inline ULong getLength() const { return getData().length(); }

            StrongPointer<Element> getNextElementSibling() const {
                auto element = getNextSibling();
                while (element && element->getNodeTypeInternal() != NodeType::ELEMENT_NODE)
                    element = element->getNextSibling();
                return std::static_pointer_cast<Element>(element);
            }

            StrongPointer<Element> getPreviousElementSibling() const {
                auto element = getPrevSibling();
                while (element && element->getNodeTypeInternal() != NodeType::ELEMENT_NODE)
                    element = element->getPrevSibling();
                return std::static_pointer_cast<Element>(element);
            }

            inline void appendData(const DOMString &toAppend) { *getValuePointer() += toAppend; }

            inline void deleteData(ULong offset, ULong count) { (*getValuePointer()).erase(offset, count); }

            inline void insertData(ULong offset, const DOMString &toInsert) {
                (*getValuePointer()).insert(offset, toInsert);
            }

            inline DOMString substringData(ULong offset, ULong count) const {
                return (*getValuePointer()).substr(offset, count);
            }

            inline void replaceData(ULong offset, ULong count, const DOMString &toReplace) {
                (*getValuePointer()).replace(offset, count, toReplace);
            }

            inline void before(const Vector <StrongPointer<Node>> &nodes) {
                getParentNode()->insertBeforeChildNDTCN(getSharedFromThis(), nodes);
            }

            inline void after(const Vector <StrongPointer<Node>> &nodes) {
                getParentNode()->insertAfterChildNDTCN(getSharedFromThis(), nodes);
            }

            inline void replaceWith(const Vector <StrongPointer<Node>> &nodes) {
                getParentNode()->replaceChildNDTCN(getSharedFromThis(), nodes);
            }

            inline void remove() {
                getParentNode()->removeChild(std::static_pointer_cast<Node>(shared_from_this()));
            }
        };
    }
}
#endif //FEATHER_CHARACTER_DATA_H
