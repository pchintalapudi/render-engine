//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_CHARACTER_DATA_H
#define FEATHER_CHARACTER_DATA_H

#include "utils/typedefs.h"
#include "node.h"
#include "ichild_node.h"
#include "inon_document_type_child_node.h"

class CharacterData : public Node, public IChildNode, public INonDocumentTypeChildNode {
public:

public:

    DOMString getData() const { return *this->data; };

    DOMString setData(std::string data) {
        delete this->data;
        this->data = new std::string(data);
    };

    const unsigned long getLength() const {
        return this->getData().length();
    }

    inline void appendData(std::string toAppend) {
        this->setData(this->getData() + toAppend);
    }

    inline void deleteData(unsigned long offset, unsigned long length) {
        this->setData(this->getData().substr(0, offset) + this->getData().substr(offset + length));
    }

    inline void insertData(unsigned long offset, std::string toInsert) {
        this->replaceData(offset, 0, toInsert);
    }

    inline void replaceData(unsigned long offset, unsigned long length, std::string toInsert) {
        this->setData(this->getData().substr(0, offset) + toInsert + this->getData().substr(offset + length));
    }

    inline std::string substringData(unsigned long offset, unsigned long length) const {
        return this->getData().substr(offset, length);
    }

    inline const std::string *getNodeValue() const override {
        return new std::string(this->getData());
    }

    inline void setNodeValue(std::string *nodeValue) override {
        this->setData(nodeValue ? *nodeValue : "");
    }

    virtual ~CharacterData() override {
        delete this->data;
    }

    void remove() override;

    void before(std::vector<Node *> &toInsert) override;

    void after(std::vector<Node *> &toInsert) override;

    void replaceWith(std::vector<Node *> &toInsert) override;

    Element *const previousElementSibling() const override;

    Element *const nextElementSibling() const override;

private:
    std::string *data;
};

#endif //FEATHER_CHARACTER_DATA_H
