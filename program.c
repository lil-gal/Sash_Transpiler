#include <stdio.h>
#include <stdlib.h> 
#include "Transpiler/Generator/Generator.h"

char *typeNames[] = {"Keyword", "TYPE", "IDENTIFIER", "EQUALS", "NUMBER", "STRING", "OPERATOR", "OPEN_PARENTHESIS", "CLOSED_PARENTHESIS", "OPEN_CURLIES", "CLOSED_CURLIES", "COMMENT", "END_OF_LINE", "END_OF_FILE", "ERROR"};
char *nodeTypeNames[] = {"VARIABLE", "FUNCTION_CREATE", "FUNCTION_CALL", "END_OF_FILE_NODE"};

char* readFile(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);
    fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

int main() {



    char fileName1[64] = "../Transpiler/file.v";
    char *readText = readFile(fileName1);
    
    

    Token *tokens = ToTokens(readText); 
    
    Node *nodes = toNodes(tokens);
    
    char *code = generateCode(nodes);
    
    //printf("%s\n", code);
    
    
    
    char fileName2[64] = "../Transpiler/file.c";
    FILE *file =  fopen(fileName2, "w");
    
    fprintf(file, "%s", code);
    fclose(file);
    
    printf("Made file\n\n");
    system("gcc ../Transpiler/file.c ../Transpiler/methods.c -o ../Transpiler/file_out && ../Transpiler/file_out");
    printf("\n\nFile executed \n");

    free(nodes);
    free(tokens);

    return 0;
}