// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include "../Lexer/Lexer.h"

typedef enum {
    VARIABLE,
    ASSIGN_VARIABLE,
    DEFINE_VARIABLE,
    FUNCTION_CREATE,
    FUNCTION_CALL,
    KEYWORD_NODE,
    END_OF_FILE_NODE
} NodeType;

typedef struct Node{
    char name[256];
    struct Node** arguments;
    int argumentCount;
    struct Node** body;
    int bodyCount;
    NodeType type;
    Token tokenType;
    Token value[64];
    int valueCount;
} Node;

Node* toNodes(Token tokens[]);
Node readNode(Token tokens[], int *index);

#endif