//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_TEXT_H
#define FEATHER_TEXT_H

#include <string>
#include "inode.h"

class Text:INode {
public:
    std::string getText();

private:
    std::string text("");
};
#endif //FEATHER_TEXT_H
