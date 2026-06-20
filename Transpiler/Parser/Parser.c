
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>
#include "Parser.h"



Node readNode(Token tokens[], int *index){
    Node n = {};

    
    TokenType type = tokens[*index].Type;
    //bool isKeyPart = false; //|| isKeyPart
    
    while(type != END_OF_LINE && type != END_OF_FILE_TOKEN && type != CLOSED_PARENTHESIS && type != CLOSED_CURLIES && type != COMMA){

        if(type == KEYWORD)                                             {
            strcpy(n.name, tokens[*index].Value);
            n.type = KEYWORD_NODE;
        }
        else if(type == TYPE)                                           { n.type = DEFINE_VARIABLE; n.tokenType = tokens[*index];}
        else if(type == OPEN_PARENTHESIS || type == CLOSED_PARENTHESIS) { n.type = FUNCTION_CALL;
            
            Node** nodes = malloc(sizeof(Node*) * 256);

            (*index)++;
            type = tokens[*index].Type;
            
            int i = 0; 
            
            while(type != CLOSED_PARENTHESIS){
                Node *node = malloc(sizeof(Node));
                *node = readNode(tokens, index);
                
                type = tokens[*index].Type;
                
                if(type == COMMA){
                    //printf("COMMAAA\n");
                    (*index)++;
                    type = tokens[*index].Type;
                }

                nodes[i] = node;    
                
                n.argumentCount = i+1;
                
                i++;
                //(*index)++;
            }
            n.arguments = nodes;

        }
        else if(type == OPEN_CURLIES || type == CLOSED_CURLIES)         { 
            if(n.type == FUNCTION_CALL) { n.type = FUNCTION_CREATE; }
            n.type = FUNCTION_CREATE;
            Node** nodes = malloc(sizeof(Node*) * 256);

            (*index)++;
            type = tokens[*index].Type;
            
            int i = 0; 

            while(type != CLOSED_CURLIES){

                Node *node = malloc(sizeof(Node));
                *node = readNode(tokens, index);
                
                type = tokens[*index].Type;
                
                if(type == END_OF_LINE){
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
        else if(type == IDENTIFIER)                                     {
            if(strlen(n.name) == 0 ){
                strcpy(n.name, tokens[*index].Value);
            }else{
                n.value[n.valueCount] = tokens[*index];
                n.valueCount++;
            }
            if(n.type != DEFINE_VARIABLE && n.type != KEYWORD_NODE && n.type != ASSIGN_VARIABLE) {
                n.type = VARIABLE;
            }
                
        }
        else if(type == STRING)                                         { n.value[n.valueCount] = tokens[*index]; n.valueCount++; }
        else if(type == NUMBER)                                         { n.value[n.valueCount] = tokens[*index]; n.valueCount++; }
        else if(type == BOOL)                                           { n.value[n.valueCount] = tokens[*index]; n.valueCount++; }
        else if(type == EQUALS)                                         { if(n.type != DEFINE_VARIABLE ) n.type = ASSIGN_VARIABLE;}
        else if(type == OPERATOR)                                       {
            n.value[n.valueCount] = tokens[*index]; n.valueCount++;
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
        if(tokens[index].Type == END_OF_LINE){
            index++;
        }

        if(tokens[index].Type == END_OF_FILE_TOKEN){
            Node n;
            n.type = END_OF_FILE_NODE;
            nodes[i] = n;
            break;
        }

        nodes[i] = readNode(tokens, &index);
        i++;
    }

    return nodes;
}