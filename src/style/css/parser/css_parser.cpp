//
// Created by binhe on 12/26/2018.
//

#include "style/css/parser/css_parser.h"

using namespace feather::css::parser;

CSSTokenizer::CSSTokenizer() {
    this->text = std::queue<std::tuple<std::shared_ptr<char[]>, size_t>>();
}

CSSTokenizer::TokenSet CSSTokenizer::processWhitespace() {
    while (std::isspace(this->get())) {
        this->advance();
    }
    auto tokenSet =  CSSTokenizer::TokenSet();
    tokenSet += (CSSTokenizer::Token::WHITESPACE);
    return tokenSet;
}

CSSTokenizer::TokenSet CSSTokenizer::processQuotation() {
    int length = 0;
    int beginning = 0;
    while (this->get() != '"') {
        switch (this->get()) {
            case '\n':
                break;
            case '\\':
                break;
            default:
                break;
        }
        this->advance();
        length++;
    }
    auto tokenSet = CSSTokenizer::TokenSet();
    tokenSet += CSSTokenizer::Token::STRING;
    return tokenSet;
}


CSSTokenizer::Token CSSTokenizer::next() {
    switch (this->get_advance()) {
        case '(':
            return CSSTokenizer::Token::LEFT_PARENTHESIS;
        case ')':
            return CSSTokenizer::Token::RIGHT_PARENTHESIS;
        case '@':
            return CSSTokenizer::Token::AT_KEYWORD;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return CSSTokenizer::Token::NUMBER;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return CSSTokenizer::Token::NUMBER;
        case ' ':
        case '\t':
        case '\f':
        case '\r':
        case '\n':
            while (std::isspace(this->get())) {
                this->advance();
            }
            return CSSTokenizer::Token::WHITESPACE;
        default:
            return CSSTokenizer::Token::EOF_TOKEN;
    }

}