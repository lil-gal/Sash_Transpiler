
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>
#include "Parser.h"


Node createNode() {
    Node n = {0};
    strcpy(n.name, "\0");
    n.value     = malloc(sizeof(Node*) * 256);
    n.arguments = malloc(sizeof(Node*) * 256);
    n.body      = malloc(sizeof(Node*) * 256);
    return n;
}

void assingValue(Node *n, Token self){
    Node node = createNode();
    node.type = NODE_VALUE;
    node.tokenType = self;
    strcpy(node.name, self.Value);
    n->value[n->valueCount] = malloc(sizeof(Node));
    *n->value[n->valueCount] = node;
    n->valueCount++;
}

Node readNode(Token tokens[], int *index){
    Node n = createNode();

    
    TokenType type = tokens[*index].Type;
    //bool isKeyPart = false; //|| isKeyPart
    
    while(type != TOKEN_END_OF_LINE && type != TOKEN_END_OF_FILE && type != TOKEN_CLOSED_PARENTHESIS && type != TOKEN_CLOSED_CURLIES && type != TOKEN_COMMA){

        if(type == TOKEN_KEYWORD)                                             {
            strcpy(n.name, tokens[*index].Value);
            n.type = NODE_KEYWORD;
        }
        else if(type == TOKEN_TYPE)                                           { n.type = NODE_DEFINE; n.tokenType = tokens[*index];}
        else if(type == TOKEN_OPEN_PARENTHESIS || type == TOKEN_CLOSED_PARENTHESIS) { 
            //printf("0-%s -\t",n.name);
            Node tempNode = n; 
            Node** nodes = malloc(sizeof(Node*) * 256);

            (*index)++;
            type = tokens[*index].Type;
            
            int i = 0; 
            
            while(type != TOKEN_CLOSED_PARENTHESIS){
                Node *node = malloc(sizeof(Node));
                *node = readNode(tokens, index);
                
                type = tokens[*index].Type;
                
                if(type == TOKEN_COMMA){
                    //printf("COMMAAA\n");
                    (*index)++;
                    type = tokens[*index].Type;
                }

                nodes[i] = node;    
                
                tempNode.argumentCount = i+1;
                
                i++;
                //(*index)++;
            }
            tempNode.type = NODE_FUNCTION_CALL;
            tempNode.arguments = nodes;
            
            if(n.type != NODE_ASSIGN_VARIABLE && n.type != NODE_DEFINE_VARIABLE){
                n.type = NODE_FUNCTION_CALL;
                n.arguments = nodes;
                n.argumentCount = i;
            }else{
                n.valueCount--; //rewrites the identifier...
                strcpy(tempNode.name, n.value[n.valueCount]->name);
                n.value[n.valueCount] = malloc(sizeof(Node));
                *n.value[n.valueCount] = tempNode;
                
                n.valueCount++;


                //printf("%d\n", n.valueCount);
            }

        }
        else if(type == TOKEN_OPEN_CURLIES || type == TOKEN_CLOSED_CURLIES)         { 
            if(n.type == NODE_FUNCTION_CALL) { n.type = NODE_FUNCTION_CREATE; }
            n.type = NODE_FUNCTION_CREATE;
            Node** nodes = malloc(sizeof(Node*) * 256);

            (*index)++;
            type = tokens[*index].Type;
            
            int i = 0; 

            while(type != TOKEN_CLOSED_CURLIES){

                Node *node = malloc(sizeof(Node));
                *node = readNode(tokens, index);
                
                type = tokens[*index].Type;
                
                if(type == TOKEN_END_OF_LINE){
                    (*index)++;
                    type = tokens[*index].Type;
                }
                
                nodes[i] = node;    

                n.bodyCount = i+1;
                
                i++;
                //(*index)++;
                
            }
            n.body = nodes;

            (*index)++;
            return n;
        }
        else if(type == TOKEN_IDENTIFIER)                                     {
            if(strlen(n.name) == 0 ){
                strcpy(n.name, tokens[*index].Value);
            }else{
                //printf("identifier print - %s\n", n.name);
                assingValue(&n, tokens[*index]);
            }

            if(n.type != NODE_DEFINE && n.type != NODE_DEFINE_VARIABLE && n.type != NODE_KEYWORD && n.type != NODE_ASSIGN_VARIABLE) {
                n.type = NODE_VALUE; //temporary?
            }
                
        }
        else if(type == TOKEN_STRING)                                         { assingValue(&n, tokens[*index]); }
        else if(type == TOKEN_NUMBER)                                         { assingValue(&n, tokens[*index]); }
        else if(type == TOKEN_BOOL)                                           { assingValue(&n, tokens[*index]); }
        else if(type == TOKEN_EQUALS)                                         { 
            if(n.type != NODE_DEFINE) {n.type = NODE_ASSIGN_VARIABLE;}
            else                 n.type = NODE_DEFINE_VARIABLE;

        }
        else if(type == TOKEN_OPERATOR)                                       {
            assingValue(&n, tokens[*index]);
        }
        
        (*index)++;
        type = tokens[*index].Type;
    }

    
    return n;

}

Node* toNodes(Token tokens[]){
    int index = 0;
    Node *nodes = malloc(sizeof(Node) * 256);

    int i = 0;
    while(true){
        // skip semicolons between statements
        if(tokens[index].Type == TOKEN_END_OF_LINE){
            index++;
        }

        if(tokens[index].Type == TOKEN_END_OF_FILE){
            Node n;
            n.type = NODE_END_OF_FILE;
            nodes[i] = n;
            break;
        }

        nodes[i] = readNode(tokens, &index);
        i++;
    }

    return nodes;
}