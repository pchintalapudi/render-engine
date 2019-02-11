//
// Created by binhe on 12/26/2018.
//

#include "style/css/parser/css_parser.h"

using namespace feather::css::parser;

CSSTokenizer::CSSTokenizer() {
    this->text = std::deque<std::tuple<std::shared_ptr<char[]>, size_t>>();
}

CSSTokenizer::TokenSet CSSTokenizer::processWhitespace() {
    while (std::isspace(this->get())) {
        this->advance();
    }
    auto tokenSet = CSSTokenizer::TokenSet();
    tokenSet += (CSSTokenizer::Token::WHITESPACE);
    return tokenSet;
}

CSSTokenizer::TokenSet CSSTokenizer::consumeStringToken(char delim) {
    std::stringstream s;
    auto token = CSSTokenizer::TokenSet();
    while (this->get() != delim) {
        switch (this->get()) {
            case '\0':
                this->token = s.str();
                token += CSSTokenizer::Token::STRING;
                return token;
            case '\n':
                token += CSSTokenizer::Token::BAD_STRING;
                return token;
            case '\\': {
                char next = this->lookahead();
                switch (next) {
                    case 0:
                        break;
                    case 1:
                        break;
                }
            }
                break;
            default:
                s << this->get();
                break;
        }
        this->advance();
        length++;
    }
    this->token = s.str();
    token += CSSTokenizer::Token::STRING;
    return token;
}

CSSTokenizer::TokenSet CSSTokenizer::processQuotation() {
    return this->consumeStringToken('"');
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