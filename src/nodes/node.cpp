//
// Created by prem on 12/11/2018.
//

#include "nodes/node.h"
#include "nodes/text/text.h"

using namespace feather::dom;

feather::UInt Node::getIndex() const {
    if (nodeIndex->isValid()) return nodeIndex->get();
    if (getParentNode()) {
        getParentNode()->updateChildIndeces();
        return nodeIndex->get();
    } else {
        nodeIndex->set(0);
        return 0;
    }
}

void Node::updateChildIndeces() const {
    auto children = getChildNodes();
    for (UInt i = 0; i < children->size(); i++) children->get(i)->nodeIndex->set(i);
}

feather::StrongPointer<Node> Node::replaceChild(feather::StrongPointer<feather::dom::Node> newChild,
                                                feather::StrongPointer<feather::dom::Node> oldChild) {
    if (newChild->getParentNode()) newChild->getParentNode()->removeChild(newChild);
    newChild->setParentNode(getSharedFromThis());
    auto idx = oldChild->getIndex();
    oldChild->clearParentNode();
    getChildNodes()->set(idx, newChild);
    return oldChild;
}

feather::StrongPointer<Node> Node::removeChild(feather::StrongPointer<feather::dom::Node> child) {
    if (child->getParentNode().get() == this) {
        child->clearParentNode();
        getChildNodes()->remove(child->getIndex());
        return child;
    }
    return StrongPointer<Node>();
}

void Node::normalize() {
    auto children = getChildNodes();
    UInt lastIndex = ~0u;
    DOMString temp;
    for (UInt i = children->size(); i-- > 0;) {
        const auto &child = children->get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::ELEMENT_NODE:
            case NodeType::SHADOW_ROOT:
            case NodeType::DOCUMENT_NODE:
            case NodeType::DOCUMENT_FRAGMENT_NODE: {
                if (~lastIndex && lastIndex > i + 1) {
                    auto reserve = temp.length();
                    auto text = Text::create(getBaseURI(), getSharedFromThis(), std::move(temp));
                    children->get(lastIndex)->clearParentNode();
                    children->removeAll(i + 1, lastIndex);
                    children->insert(i + 1, std::move(text));
                    temp = "";
                    temp.reserve(reserve);
                    lastIndex = ~0u;
                }
                break;
            }
            case NodeType::TEXT_NODE: {
                auto val = *child->getNodeValue();
                temp.insert(temp.begin(), val.begin(), val.end());
                if (~lastIndex) {
                    child->clearParentNode();
                } else lastIndex = i;
                break;
            }
            default:
                break;
        }
    }
    if (~lastIndex && lastIndex > 0) {
        auto text = Text::create(getBaseURI(), getSharedFromThis(), std::move(temp));
        children->get(lastIndex)->clearParentNode();
        children->removeAll(0, lastIndex);
        children->insert(0, std::move(text));
    }
}

feather::StrongPointer<Node> Node::insertAfter(feather::StrongPointer<feather::dom::Node> add,
                                               feather::StrongPointer<const feather::dom::Node> ref) {
    if (ref->getParentNode().get() == this) {
        if (add->getParentNode()) add->getParentNode()->removeChild(add);
        add->setParentNode(getSharedFromThis());
        getChildNodes()->insert(ref->getIndex() + 1, add);
        return add;
    }
    return StrongPointer<Node>();
}

feather::StrongPointer<Node> Node::insertBefore(feather::StrongPointer<feather::dom::Node> add,
                                                feather::StrongPointer<const feather::dom::Node> ref) {
    if (ref->getParentNode().get() == this) {
        if (ref->getNodeTypeInternal() == NodeType::DOCUMENT_FRAGMENT_NODE) {
            auto children = add->getChildNodes();
            auto size = getChildNodes()->size();
            getChildNodes()->insertAll(ref->getIndex(), *children);
            for (UInt i = 0; i < children->size(); i++) {
                auto child = children->get(i);
                child->setParentNode(getSharedFromThis());
                child->nodeIndex->set(i + size);
            }
            children->clear();
            return add;
        } else {
            if (add->getParentNode()) add->getParentNode()->removeChild(add);
            add->setParentNode(getSharedFromThis());
            getChildNodes()->insert(ref->getIndex(), add);
            return add;
        }
    }
    return StrongPointer<Node>();
}

feather::StrongPointer<Node> Node::getRootNode(bool composed) const {
    StrongPointer <Node> parent = getParentNode();
    //TODO: Make this better
    if (!parent) return std::const_pointer_cast<Node>(getSharedFromThis());
    while (parent->getParentNode() && (composed || parent->getNodeTypeInternal() != NodeType::SHADOW_ROOT))
        parent = parent->getParentNode();
    return parent;
}

bool Node::contains(const feather::StrongPointer<const feather::dom::Node> &other) const {
    if (other.get() == this) return true;
    auto parent = other->getParentNode();
    while (parent && parent.get() != this) parent = parent->getParentNode();
    return parent != nullptr;
}

namespace {
    enum DocumentPosition {
        DISCONNECTED = 1u,
        PRECEDING = 1u << 1u,
        FOLLOWING = 1u << 2u,
        CONTAINS = 1u << 3u,
        CONTAINED_BY = 1u << 4u,
        IMPLEMENTATION_SPECIFIC = 1u << 5u
    };

    const feather::dom::Node *
    getPrecedence(const feather::dom::Node *const &root, const feather::dom::Node *const &node1,
                  const feather::dom::Node *const &node2) {
        const auto &children = root->getChildNodes();
        for (feather::UInt i = 0; i < children->size(); i++) {
            const auto &child = children->get(i);
            if (child.get() == node1) return node1;
            if (child.get() == node2) return node2;
            if (child->hasChildNodes()) {
                auto precedence = getPrecedence(child.get(), node1, node2);
                if (precedence) return precedence;
            }
        }
        return nullptr;
    }

    bool precedes(const feather::StrongPointer<const feather::dom::Node> &node1,
                  const feather::StrongPointer<const feather::dom::Node> &node2) {
        return getPrecedence(node1->getRootNode().get(), node1.get(), node2.get()) == node1.get();
    }
}

unsigned char Node::compareDocumentPosition(const feather::StrongPointer<const feather::dom::Node> &other) const {
    if (other.get() == this) return 0u;
    if (!other || other->getRootNode(true) != getRootNode(true))
        return DocumentPosition::DISCONNECTED | DocumentPosition::PRECEDING | DocumentPosition::IMPLEMENTATION_SPECIFIC;
    if (other->contains(getSharedFromThis())) return DocumentPosition::CONTAINS | DocumentPosition::PRECEDING;
    if (contains(other)) return DocumentPosition::CONTAINED_BY | DocumentPosition::FOLLOWING;
    if (precedes(other, getSharedFromThis())) return DocumentPosition::PRECEDING;
    return DocumentPosition::FOLLOWING;
}

feather::StrongPointer<Node> Node::appendChild(feather::StrongPointer<feather::dom::Node> child) {
    if (child->getParentNode()) child->getParentNode()->removeChild(child);
    child->setParentNode(getSharedFromThis());
    getChildNodes()->add(child);
    return child;
}

void Node::setTextContent(feather::DOMString textContent) {
    switch (getNodeTypeInternal()) {
        case NodeType::SHADOW_ROOT:
        case NodeType::DOCUMENT_FRAGMENT_NODE:
        case NodeType::ELEMENT_NODE:
            getChildNodes()->clear();
            getChildNodes()->add(Text::create(getBaseURI(), getSharedFromThis(), std::move(textContent)));
            break;
        case NodeType::TEXT_NODE:
        case NodeType::COMMENT_NODE:
        case NodeType::PROCESSING_INSTRUCTION_NODE:
            *this->value = std::move(textContent);
            break;
        default:
            break;
    }
}

feather::StrongPointer<feather::DOMString> Node::getTextContent() const {
    switch (getNodeTypeInternal()) {
        case NodeType::TEXT_NODE:
            return getNodeValue();
        case NodeType::ELEMENT_NODE:
        case NodeType::DOCUMENT_FRAGMENT_NODE:
        case NodeType::DOCUMENT_NODE:
            return std::make_shared<DOMString>(getTextContentInternal());
        default:
            return StrongPointer<DOMString>();
    }
}

feather::DOMString Node::getTextContentInternal() const {
    switch (getNodeTypeInternal()) {
        case NodeType::TEXT_NODE:
            return *value;
        case NodeType::ELEMENT_NODE:
        case NodeType::DOCUMENT_NODE:
        case NodeType::DOCUMENT_FRAGMENT_NODE:
        case NodeType::SHADOW_ROOT: {
            auto children = getChildNodes();
            DOMString temp;
            for (UInt i = 0; i < children->size(); i++) temp += children->get(i)->getTextContentInternal();
            return temp;
        }
        default:
            return "";
    }
}

feather::StrongPointer<feather::dom::Node> Node::getPrevSibling() const {
    auto idx = getIndex();
    return idx ? getChildNodes()->get(idx - 1) : StrongPointer<Node>();
}

feather::StrongPointer<Element> Node::getParentElement() const {
    auto parent = getParentNode();
    return parent->getNodeTypeInternal() == NodeType::ELEMENT_NODE ||
           parent->getNodeTypeInternal() == NodeType::SHADOW_ROOT
           ? std::static_pointer_cast<Element>(parent)
           : StrongPointer<Element>();
}

void Node::setParentNode(const feather::StrongPointer<feather::dom::Node> &parentNode) {
    auto old = parent.get().lock();
    if (old) old->getChildNodes()->unbind(nodeIndex);
    parent.set(parentNode);
    parentNode->getChildNodes()->bind(nodeIndex);
}

feather::StrongPointer<feather::dom::Document> Node::getOwnerDocument() const {
    if (ownerPtr->isValid()) return ownerPtr->get().lock();
    auto owner = getParentNode() ? getParentNode()->getOwnerDocument() : StrongPointer<Document>();
    ownerPtr->set(owner);
    return owner;
}

feather::StrongPointer<feather::dom::Node> Node::getNextSibling() const {
    auto idx = getIndex();
    return idx < getChildNodes()->size() ? getChildNodes()->get(idx) : StrongPointer<Node>();
}

void Node::insertBeforeChildNDTCN(feather::StrongPointer<const feather::dom::Node> ref,
                                  feather::Vector<std::shared_ptr<feather::dom::Node>> add) {
    if (ref->getParentNode().get() == this) {
        UInt idx;
        if (ref->nodeIndex->isValid()) idx = ref->nodeIndex->get();
        else {
            auto children = getChildNodes();
            idx = ~0u;
            while (++idx < children->size() && children->get(idx) != ref) children->get(idx)->nodeIndex->set(idx);
        }
        for (UInt i = 0; i < add.size(); i++) {
            auto child = add[i];
            if (child->getParentNode()) child->getParentNode()->removeChild(child);
            child->setParentNode(getSharedFromThis());
            child->nodeIndex->set(idx + i);
        }
        getChildNodes()->insertAll(idx, std::move(add));
    }
}

void Node::insertAfterChildNDTCN(feather::StrongPointer<const feather::dom::Node> ref,
                                 feather::Vector<std::shared_ptr<feather::dom::Node>> add) {
    if (ref->getParentNode().get() == this) {
        UInt idx;
        if (ref->nodeIndex->isValid()) idx = ref->nodeIndex->get() + 1;
        else {
            auto children = getChildNodes();
            idx = 0;
            while (idx < children->size() && children->get(idx++) != ref);
        }
        for (UInt i = 0; i < add.size(); i++) {
            auto child = add[i];
            if (child->getParentNode()) child->getParentNode()->removeChild(child);
            child->setParentNode(getSharedFromThis());
            child->nodeIndex->set(idx + i);
        }
        getChildNodes()->insertAll(idx, std::move(add));
    }
}

void Node::replaceChildNDTCN(feather::StrongPointer<const feather::dom::Node> ref,
                             feather::Vector<std::shared_ptr<feather::dom::Node>> add) {
    if (ref->getParentNode().get() == this) {
        UInt idx;
        if (ref->nodeIndex->isValid()) idx = ref->nodeIndex->get();
        else {
            auto children = getChildNodes();
            idx = 0;
            while (idx < children->size() && children->get(idx++) != ref);
        }
        for (UInt i = 0; i < add.size(); i++) {
            auto child = add[i];
            if (child->getParentNode()) child->getParentNode()->removeChild(child);
            child->setParentNode(getSharedFromThis());
            child->nodeIndex->set(idx + i);
        }
        getChildNodes()->insertAll(idx, std::move(add));
        getChildNodes()->remove(idx - 1)->clearParentNode();
    }
}

Node::Node(feather::DOMString baseURI, feather::DOMString name, feather::dom::NodeType type,
           feather::StrongPointer<feather::DOMString> value,
           const feather::StrongPointer<feather::dom::Node> &parent) : baseURI(std::move(baseURI)),
                                                                       name(std::move(name)), type(type),
                                                                       value(std::move(value)) {
    this->parent.bind(ownerPtr);
    this->parent.bind(nodeIndex);
    setParentNode(parent);
}

void Node::modify(feather::RegularEnumSet<feather::observable::InvEvent> &types,
                  const feather::observable::Invalidatable *source) const {
    //TODO: Actually implement me
    types.add(observable::InvEvent::INVALIDATED);
}