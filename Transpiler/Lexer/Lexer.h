// Lexer.h
#ifndef LEXER_H
#define LEXER_H

typedef enum{
    KEYWORD,
    TYPE,
    IDENTIFIER,
    EQUALS,
    NUMBER,
    STRING,
    BOOL,
    OPERATOR,
    COMMA,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
    OPEN_CURLIES,
    CLOSED_CURLIES,
    COMMENT,
    END_OF_LINE,
    END_OF_FILE_TOKEN,
    ERROR
}TokenType;

typedef struct{
    TokenType Type;
    char Value[256];
}Token;

Token toToken(char ch[], int *index);
Token* ToTokens(char ch[]);

#endif