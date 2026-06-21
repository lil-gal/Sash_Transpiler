
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
    node.type = VALUE;
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
    
    while(type != END_OF_LINE && type != END_OF_FILE_TOKEN && type != CLOSED_PARENTHESIS && type != CLOSED_CURLIES && type != COMMA){

        if(type == KEYWORD)                                             {
            strcpy(n.name, tokens[*index].Value);
            n.type = KEYWORD_NODE;
        }
        else if(type == TYPE)                                           { n.type = DEFINE; n.tokenType = tokens[*index];}
        else if(type == OPEN_PARENTHESIS || type == CLOSED_PARENTHESIS) { 
            //printf("0-%s -\t",n.name);
            Node tempNode = n; 
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
                
                tempNode.argumentCount = i+1;
                
                i++;
                //(*index)++;
            }
            tempNode.type = FUNCTION_CALL;
            tempNode.arguments = nodes;
            
            if(n.type != ASSIGN_VARIABLE && n.type != DEFINE_VARIABLE){
                n.type = FUNCTION_CALL;
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
                //printf("identifier print - %s\n", n.name);
                assingValue(&n, tokens[*index]);
            }

            if(n.type != DEFINE && n.type != DEFINE_VARIABLE && n.type != KEYWORD_NODE && n.type != ASSIGN_VARIABLE) {
                n.type = VALUE; //temporary?
            }
                
        }
        else if(type == STRING)                                         { assingValue(&n, tokens[*index]); }
        else if(type == NUMBER)                                         { assingValue(&n, tokens[*index]); }
        else if(type == BOOL)                                           { assingValue(&n, tokens[*index]); }
        else if(type == EQUALS)                                         { 
            if(n.type != DEFINE) {n.type = ASSIGN_VARIABLE;}
            else                 n.type = DEFINE_VARIABLE;

        }
        else if(type == OPERATOR)                                       {
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