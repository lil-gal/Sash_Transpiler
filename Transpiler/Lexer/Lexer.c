//Lexer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>

#include "Lexer.h"

bool isNumber(char ch[]){
    for (int i = 0; i < strlen(ch); i++) {
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


Token toToken(char ch[], int *index){
    
    //END OF FILE
    if(*index >= strlen(ch)) {
        Token t = {END_OF_FILE_TOKEN, ""};
        return t; 
    }


    char firstchar = ch[*index];
    

    //IF SPACE
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
        Token t = {STRING, ""};
        strcpy(t.Value, strWord);
        return t;
    }

    //SEMICOLON
    if(firstchar == ';'){
        (*index)++;
        Token t = {END_OF_LINE, ";"};
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
            Token t = {OPERATOR, ""};
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
        Token t = {EQUALS, "="};
        if(ch[*index] == '='){
            (*index)++;
            t.Type = OPERATOR;
            strcpy(t.Value, "==");
        }
        return t;
    }

    //EXCLAMATION MARK
    if(firstchar == '!'){
        (*index)++;
        Token t = {OPERATOR, "!"};
        if(ch[*index] == '='){
            (*index)++;
            t.Type = OPERATOR;
            strcpy(t.Value, "!=");
        }
        return t;
    }

    //COMMA
    if(firstchar == ','){
        (*index)++;
        Token t = {COMMA, ","};
        return t;
    }

    

    //braces
    char braces[] = {'(', ')', '{', '}'};
    for(int i = 0; i < sizeof(braces); i++){
        Token t = {OPEN_PARENTHESIS, ""};
        char b = braces[i];

        if(firstchar == b){
            if(b == '(')            { t.Type = OPEN_PARENTHESIS; }
            else if(b == ')')       { t.Type = CLOSED_PARENTHESIS; }
            else if(b == '{')       { t.Type = OPEN_CURLIES; }
            else if(b == '}')       { t.Type = CLOSED_CURLIES; }
            (*index)++;
            char val[2] = {firstchar, '\0'};
            strcpy(t.Value, val);
            return t;
        }
    }

    



    int startIndex = *index;
    int currIndex = *index;
    char currWord[64] = "";

    while(true){
        char c = ch[currIndex]; 
        

        bool shouldEnd = false;
        if(c == ' ' || c == '\0')           { shouldEnd = true; }
        else if(c == '(' || c == '{')       { shouldEnd = true; }
        else if(c == ')' || c == '}')       { shouldEnd = true; }
        else if(c == ';')                   { shouldEnd = true; }
        else if(c == ',')                   { shouldEnd = true; }
        else if(c == '/' || c == '*')       { shouldEnd = true; }
        else if(c == '-' || c == '+')       { shouldEnd = true; }
        else if(c == '=' || c == '!')       { shouldEnd = true; }
        else if(c == "\n")                  { shouldEnd = true; }


        if(shouldEnd) {
            Token t = {IDENTIFIER, ""};

            //block timeee

            if(strcmp(currWord, "int") == 0)          { t.Type = TYPE; }
            else if(strcmp(currWord, "string") == 0)  { t.Type = TYPE; }
            else if(strcmp(currWord, "char") == 0)    { t.Type = TYPE; }
            else if(strcmp(currWord, "bool") == 0)    { t.Type = TYPE; }
            else if(strcmp(currWord, "void") == 0)    { t.Type = TYPE; }
            else if(strcmp(currWord, "true") == 0)    { t.Type = BOOL; }
            else if(strcmp(currWord, "false") == 0)   { t.Type = BOOL; }
            else if(isNumber(currWord))               { t.Type = NUMBER;  }
            else if(isKeyword(currWord))              { t.Type = KEYWORD; }
            //else                                      { t.Type = IDENTIFIER; }



            strcpy(t.Value, currWord);
            *index = currIndex;
            return t;
        }


        currWord[currIndex - startIndex] = ch[currIndex];
        //currWord[currIndex - startIndex] = '\0';
        currIndex++;
    }
}


Token* ToTokens(char ch[]){
    int index = 0;
    Token *tokens = malloc(sizeof(Token) * 256);

    int i = 0;

    while(true){
        tokens[i] = toToken(ch, &index);
        
        if(tokens[i].Type == END_OF_FILE_TOKEN){
            break;
        }

        i++;
    }

    return tokens;

}