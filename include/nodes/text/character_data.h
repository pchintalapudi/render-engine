//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_CHARACTER_DATA_H
#define FEATHER_CHARACTER_DATA_H

#include "../node.h"

namespace feather {
    namespace dom {
        class CharacterData : public Node {
        public:

            CharacterData(DOMString &&baseURI, DOMString &&name, NodeType type, const StrongPointer <Node> &parent)
                    : Node(DOMString(baseURI), DOMString(name), type, std::make_shared<DOMString>(), parent) {}

            CharacterData(DOMString &&baseURI, DOMString &&name, NodeType type, const StrongPointer <Node> &parent,
                          DOMString &&initText)
                    : Node(DOMString(baseURI), DOMString(name), type, std::make_shared<DOMString>(), parent) {
                setData(DOMString(initText));
            }

            DOMString getData() const { return *getValuePointer(); }

            inline void setData(DOMString &&data) { *getValuePointer() = data; }

            inline ULong getLength() const { return getData().length(); }

            inline StrongPointer <Element> getNextElementSibling() const {
                return getElementAfterChild(std::static_pointer_cast<const Node>(shared_from_this()));
            }

            inline StrongPointer <Element> getPreviousElementSibling() const {
                return getElementBeforeChild(std::static_pointer_cast<const Node>(shared_from_this()));
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
                this->getParentNode()->insertBeforeChild(nodes, std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void after(const Vector <StrongPointer<Node>> &nodes) {
                this->getParentNode()->insertAfterChild(nodes, std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void replaceWith(const Vector <StrongPointer<Node>> &nodes) {
                this->getParentNode()->replaceChild(nodes, std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void remove() {
                getParentNode()->removeChild(std::static_pointer_cast<Node>(shared_from_this()));
            }
        };
    }
}
#endif //FEATHER_CHARACTER_DATA_H
