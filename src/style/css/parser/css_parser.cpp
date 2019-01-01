//
// Created by binhe on 12/26/2018.
//

#include "style/css/parser/css_parser.h"

using namespace feather::css::parser;

CSSTokenizer::CSSTokenizer() {
    this->text = std::queue<std::tuple<std::shared_ptr<char[]>, size_t>>();
}

CSSTokenizer::Token CSSTokenizer::next() {
    this->index++;
    if (this->index >= this->get_length()) {
        this->index -= this->get_length();
        this->text.pop();
    }
    switch (this->get()) {
        case '(':
            return CSSTokenizer::Token::LEFT_PARENTHESIS;
        case ')':
            return CSSTokenizer::Token::RIGHT_PARENTHESIS;
        case '*':
            return CSSTokenizer::Token ::ASTERISK;
        case '@':
            return CSSTokenizer::Token ::AT_KEYWORD;
        case ' ':
            return CSSTokenizer::Token ::WHITESPACE;
        default:
            return CSSTokenizer::Token::EOF_TOKEN;
    }

}