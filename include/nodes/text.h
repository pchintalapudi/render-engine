//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_TEXT_H
#define FEATHER_TEXT_H

#include <string>
#include "character_data.h"
#include "node.h"

class Text : public CharacterData {
public:

    std::string getData() const override {
        return this->text;
    }

    std::string setData(std::string data) override {
        this->text = data;
    }

    ~Text() override {}

private:
    std::string text;
};

#endif //FEATHER_TEXT_H
