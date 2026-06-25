// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include "../Lexer/Lexer.h"

typedef enum {
    NODE_VALUE,
    NODE_ASSIGN_VARIABLE,
    NODE_DEFINE,
    NODE_DEFINE_VARIABLE,
    NODE_FUNCTION_CREATE,
    NODE_FUNCTION_CALL,
    NODE_KEYWORD,
    NODE_END_OF_FILE
} NodeType;

typedef struct Node{
    char name[256];
    struct Node** arguments;
    int argumentCount;
    struct Node** body;
    int bodyCount;
    NodeType type;
    Token tokenType;
    struct Node** value;
    int valueCount;
} Node;

Node* toNodes(Token tokens[]);
Node readNode(Token tokens[], int *index);

#endif