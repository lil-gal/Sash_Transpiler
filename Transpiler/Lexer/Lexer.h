// Lexer.h
#ifndef LEXER_H
#define LEXER_H

typedef enum{
    TOKEN_KEYWORD,
    TOKEN_TYPE,
    TOKEN_IDENTIFIER,
    TOKEN_EQUALS,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_BOOL,
    TOKEN_OPERATOR,
    TOKEN_COMMA,
    TOKEN_OPEN_PARENTHESIS,
    TOKEN_CLOSED_PARENTHESIS,
    TOKEN_OPEN_CURLIES,
    TOKEN_CLOSED_CURLIES,
    TOKEN_COMMENT,
    TOKEN_END_OF_LINE,
    TOKEN_END_OF_FILE,
    TOKEN_ERROR
}TokenType;

typedef struct{
    TokenType Type;
    char Value[256];
}Token;

Token toToken(char *ch, int *index);
Token* ToTokens(char *ch);

#endif