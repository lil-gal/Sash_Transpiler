//Lexer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>

#include "Lexer.h"

bool isNumber(char ch[]){
    for (int i = 0; ch[i] != '\0'; i++) {
        if(!isdigit(ch[i])) { return false; }
    }
    return true;
    
}

bool isKeyword(char ch[]){
         if(strcmp(ch, "ret") == 0)    {return true;}
    else if(strcmp(ch, "if") == 0)     {return true;}
    else if(strcmp(ch, "elif") == 0)   {return true;}
    else if(strcmp(ch, "else") == 0)   {return true;}
    else if(strcmp(ch, "while") == 0)  {return true;}
    
    
    return false;
}

Token checkOutside(char ch[], int *index){
    char firstchar = ch[*index];
    

    //IF SPACE OR NEWLINE
    if(firstchar == ' ' || firstchar == '\n') {
        (*index)++;
        return toToken(ch, index); 
    }

    //STRINGS
    if(ch[*index] == '\"'){
        (*index)++;
        char strWord[256] = "";
        int i = 0;
        while(ch[*index] != '\"' && ch[*index] != '\0'){
            strWord[i] = ch[*index];
            (*index)++;
            i++;
        }
        (*index)++; 
        Token t = {TOKEN_STRING, ""};
        strcpy(t.Value, strWord);
        return t;
    }

    //SEMICOLON
    if(firstchar == ';'){
        (*index)++;
        Token t = {TOKEN_END_OF_LINE, ";"};
        return t;
    }

    

    //OPERATORS  -   COMMENTS
    char operators[] = {'+', '-', '/', '*'};
    if(firstchar == '/' && ch[*index + 1] == '/'){
        int i = (*index);
        while(ch[i] != '\n'){
            i++;
        }

        (*index) = i;
        return toToken(ch, index);
    }
    for(int i = 0; i < sizeof(operators); i++){
        if(firstchar == operators[i]){
            (*index)++;
            Token t = {TOKEN_OPERATOR, ""};
            char val[2] = {firstchar, '\0'};
            strcpy(t.Value, val);
            return t;
        }
    }

    //IF \n
    if(firstchar == '\n') {
        (*index)++;
        return toToken(ch, index); 
    }

    //EQUALS SIGN
    if(firstchar == '='){
        (*index)++;
        Token t = {TOKEN_EQUALS, "="};
        if(ch[*index] == '='){
            (*index)++;
            t.Type = TOKEN_OPERATOR;
            strcpy(t.Value, "==");
        }
        return t;
    }

    //EXCLAMATION MARK
    if(firstchar == '!'){
        (*index)++;
        Token t = {TOKEN_OPERATOR, "!"};
        if(ch[*index] == '='){
            (*index)++;
            t.Type = TOKEN_OPERATOR;
            strcpy(t.Value, "!=");
        }
        return t;
    }

    //TOKEN_COMMA
    if(firstchar == ','){
        (*index)++;
        Token t = {TOKEN_COMMA, ","};
        return t;
    }

    

    //braces
    char braces[] = {'(', ')', '{', '}'};
    for(int i = 0; i < sizeof(braces); i++){
        Token t = {TOKEN_OPEN_PARENTHESIS, ""};
        char b = braces[i];

        if(firstchar == b){
            if(b == '(')            { t.Type = TOKEN_OPEN_PARENTHESIS; }
            else if(b == ')')       { t.Type = TOKEN_CLOSED_PARENTHESIS; }
            else if(b == '{')       { t.Type = TOKEN_OPEN_CURLIES; }
            else if(b == '}')       { t.Type = TOKEN_CLOSED_CURLIES; }
            (*index)++;
            char val[2] = {firstchar, '\0'};
            strcpy(t.Value, val);
            return t;
        }
    }

    printf("ended without returning. Lexer - checkOutside");
}

bool shouldEndFn(char c){
    bool shouldEnd = false;
    
    // block of bad characters :(
    if(c == ' ' || c == '\0')           { shouldEnd = true; }
    else if(c == '(' || c == '{')       { shouldEnd = true; }
    else if(c == ')' || c == '}')       { shouldEnd = true; }
    else if(c == ';')                   { shouldEnd = true; }
    else if(c == ',')                   { shouldEnd = true; }
    else if(c == '/' || c == '*')       { shouldEnd = true; }
    else if(c == '-' || c == '+')       { shouldEnd = true; }
    else if(c == '=' || c == '!')       { shouldEnd = true; }
    else if(c == '\n')                  { shouldEnd = true; }

    return shouldEnd;
}

void changeType(char currWord[], Token *t){

         if(strcmp(currWord, "int") == 0)     { t->Type = TOKEN_TYPE; }
    else if(strcmp(currWord, "string") == 0)  { t->Type = TOKEN_TYPE; }
    else if(strcmp(currWord, "char") == 0)    { t->Type = TOKEN_TYPE; }
    else if(strcmp(currWord, "bool") == 0)    { t->Type = TOKEN_TYPE; }
    else if(strcmp(currWord, "void") == 0)    { t->Type = TOKEN_TYPE; }
    else if(strcmp(currWord, "true") == 0)    { t->Type = TOKEN_BOOL; }
    else if(strcmp(currWord, "false") == 0)   { t->Type = TOKEN_BOOL; }
    else if(isNumber(currWord))               { t->Type = TOKEN_NUMBER;  }
    else if(isKeyword(currWord))              { t->Type = TOKEN_KEYWORD; }
    //else                                      { t.Type = IDENTIFIER; }

}

Token toToken(char ch[], int *index){
    
    //END OF FILE
    if(*index >= strlen(ch)) {
        Token t = {TOKEN_END_OF_FILE, ""};
        return t; 
    }

    if(shouldEndFn(ch[*index])){
        return checkOutside(ch, index);
    }

    int startIndex = *index;
    int currIndex = *index;
    
    int capacity = 16;
    char *currWord = calloc(capacity, sizeof(char));

    while(true){
        bool shouldEnd = shouldEndFn(ch[currIndex]);

        int len = currIndex - startIndex;

        if(shouldEnd) {
            currWord[len] = '\0';   

            Token t = {TOKEN_IDENTIFIER, ""};

            changeType(currWord, &t);
            strcpy(t.Value, currWord);

            *index = currIndex;

            free(currWord);
            return t;
        }

        currWord[currIndex - startIndex] = ch[currIndex];
        
        if(len >= capacity-1){
            capacity *= 2;
            
            char *tmp = realloc(currWord, capacity);
            if (!tmp) {
                free(currWord);
                exit(EXIT_FAILURE);
            }
            currWord = tmp;
        }
        currIndex++;
    }
}


Token* ToTokens(char ch[]){
    int index = 0;
    int capacity = 256;

    Token *tokens = malloc(sizeof(Token) * capacity);
    
    int i = 0;
    
    while(true){
        if(i >= capacity){
            capacity *= 2;
            tokens = realloc(tokens, sizeof(Token) * capacity);
        }

        tokens[i] = toToken(ch, &index);
        
        if(tokens[i].Type == TOKEN_END_OF_FILE){
            break;
        }

        i++;
    }

    return tokens;

}