#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>
#include "Generator.h"

char* generateCode(Node nodes[]);
void handleString(char *line, Token token);
void handleFunctionCall(char *line, Node node);
void handleFunctionCreate(char *line, Node node);
void handleDefineVariable(char *line, Node node);
void handleAssignVariable(char *line, Node node);
void handleVariable(char *line, Node node);
void handleTypes(char *line, Node node);
void strcats(char *bef, char *aft);
void handleVarOrType(char* line, Node node);
void writeArguments(char *line, Node node);
void writeArgumentsWithTypes(char *line, Node node);
void writeValues(char* line, Node node);
void handleKeywords(char* line, Node node);
void handleNodeType(char* line, Node node);
void handleDefineType(char *line, Node node);


char includings[256] = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <stdbool.h>\n#include <ctype.h>\n#include \"methods.h\"\n\0";
char methodsDeclaration[256] = ""; 
char methodsCreated[256] = "";

char mainStart[24]= "int main(){\n\0";
char mainEnd[24] = "\nreturn 0;}\n\0";

void handleFunctionCall(char *line, Node node){
    char write[256];
    write[0] = '\0';

    strcat(write, node.name);
    
    writeArguments(write, node);

    strcpy(line, write);
    

}

void writeArguments(char *line, Node node){
    char write[256];
    write[0] = '\0';

    char arguments[256];
    arguments[0] = '\0';
    for(int i = 0; i < node.argumentCount; i++){
        char argument[256];
        argument[0] = '\0';
        
        handleVarOrType(argument, *node.arguments[i]);        


        //strcpy(argument, node.arguments[i]->value.Value);
        
        if(i != node.argumentCount-1){
            char d [2] = {',', '\0'};
            strcat(argument, d);
        }
        
        strcat(arguments, argument);
        
    }
    char ob[2] = {'(', '\0'};
    char cb[2] = {')', '\0'};

    strcat(write, ob);
    strcat(write, arguments);
    strcat(write, cb);
    strcat(line, write);
}

void writeArgumentsWithTypes(char *line, Node node){
    char write[256];
    write[0] = '\0';

    char arguments[256];
    arguments[0] = '\0';
    for(int i = 0; i < node.argumentCount; i++){
        char argument[256];
        argument[0] = '\0';
        
        
        handleDefineType(argument, *node.arguments[i]);
        //handleVarOrType(argument, *node.arguments[i]); 
        
        
        //strcpy(argument, node.arguments[i]->value.Value);
        
        if(i != node.argumentCount-1){
            char d [2] = {',', '\0'};
            strcat(argument, d);
        }
        
        strcat(arguments, argument);
        
    }
    char ob[2] = {'(', '\0'};
    char cb[2] = {')', '\0'};

    strcat(write, ob);
    strcat(write, arguments);
    strcat(write, cb);
    strcat(line, write);
}

void handleVarOrType(char* line, Node node){
    bool isVar = strcmp(node.name, "");  
    if(isVar){
        handleVariable(line, node);  
    }else{
        handleTypes(line, node);
    }
}

void handleVariable(char *line, Node node){
    strcats(line, node.name);
}

void handleTypes(char *line, Node node){
    for(int i = 0; i < node.valueCount; i++){
        if(node.value[i].Type == STRING){
            handleString(line, node.value[i]);
        }else{
            strcats(line, node.value[i].Value);
        }
    }
}

void handleAssignVariable(char *line, Node node){
    strcat(line, node.name);
    strcat(line, "=");

    writeValues(line, node);
}

void handleDefineVariable(char *line, Node node){
    char c[256];
    c[0] = '\0';

    handleDefineType(c, node);

    
    strcats(c, "=");                       // = 

    handleTypes(c, node);
    
    
    strcpy(line, c); 
}

void handleDefineType(char* line, Node node){
    if(strcmp(node.tokenType.Value, "string") == 0){
        strcat(line, "char");
        
        strcat(line,"*");
        
        strcats(line, node.name); 
    }else{
        strcat(line, node.tokenType.Value);  // string
        strcats(line, node.name);            // s
    }
}

void strcats(char *bef, char *aft){
    char c[256];
    c[0] = '\0';
    
    //strcat(c, bef);
    
    //char s[2] = {' ', '\0'};
    strcat(c, " ");
    
    strcat(c,aft);

    strcat(bef,c);

}

void handleString(char *line, Token token){
    char write[256];
    write[0] = '\0';
    strcat(write, "\"");
    strcat(write, token.Value);
    strcat(write, "\"");
    strcat(line, write);
}

void handleFunctionCreate(char *line, Node node){
    char c[256];
    c[0] = '\0';

    //printf("%s\n",node.tokenType.Value);
    if(strcmp(node.tokenType.Value, "string") == 0){
        strcat(c, "char");
        
        strcat(c,"*");
        
        strcats(c, node.name); 
    }else{
        strcat(c, node.tokenType.Value);  // string
        strcats(c, node.name);            // s
    }
    writeArgumentsWithTypes(c, node);
    
    char body[256] = "\0";
    
    
    strcat(body, "{\n\0");
    
    char bodyItems[256];
    bodyItems[0] = '\0';
    for(int i = 0; i < node.bodyCount; i++){
        char bodyItem[256];
        bodyItem[0] = '\0';
        
        handleNodeType(bodyItem, *node.body[i]);        


        //strcpy(argument, node.arguments[i]->value.Value);
        
        
        char d [3] = {';', '\n' , '\0'};
        strcat(bodyItem, d);
        
        //handleNodeType(body, );
        //strcat(bodyItem, ";");
        
        strcat(bodyItems, bodyItem);
        
    }
    strcat(body, bodyItems);

    if(strcmp(node.name, "main") != 0){
        strcat(methodsDeclaration, c);
        strcat(methodsDeclaration, ";\n");
    }


    strcat(body, "}\n");
    strcat(c, body);

    strcat(methodsCreated, c);

    //strcat(line, c);
}

void writeValues(char* line, Node node){
    for(int i = 0; i < node.valueCount; i++){
        if(node.value[i].Type == STRING){
            char c[256];
            c[0] = '\0';
            sprintf(c, "\"%s\"", node.value[i].Value);
            strcats(line, c);
        }else{
            strcats(line, node.value[i].Value);
        }
    }
        
}

void handleKeywords(char* line, Node node){
    if(strcmp(node.name, "ret") == 0){
        strcat(line, "return");

        writeValues(line, node);
    }
}


void handleNodeType(char* line, Node node){
    NodeType type = node.type;
         if(type == FUNCTION_CALL)       { handleFunctionCall(line,   node); }
    else if(type == DEFINE_VARIABLE)     { handleDefineVariable(line, node); }
    else if(type == ASSIGN_VARIABLE)     { handleAssignVariable(line, node); }
    else if(type == FUNCTION_CREATE)     { handleFunctionCreate(line, node); }
    else if(type == KEYWORD_NODE)        { handleKeywords(line, node); }
}


char* generateCode(Node nodes[]) {
    char *code = malloc(sizeof(char) * 4096);  // bigger
    code[0] = '\0';

    //strcat(code, mainStart);

    int i = 0;
    while (true) {
        char line[1024];
        line[0] = '\0';
        NodeType type = nodes[i].type;

        if (type == END_OF_FILE_NODE) {
            //strcat(code, mainEnd);
            break;  // break, don't return yet
        }

        handleNodeType(line, nodes[i]);

        if(type != FUNCTION_CREATE){
            char sc[3] = {';', '\n', '\0'};
            strcat(line, sc);
        }else{
            char sc[2] = {'\n', '\0'};
            strcat(line, sc);
        }

        strcat(code, line);
        i++;
    }

    // NOW assemble the full file
    char *fileText = malloc(sizeof(char) * 8192);
    fileText[0] = '\0';

    strcat(fileText, includings);
    strcat(fileText, methodsDeclaration);  // forward declarations first
    strcat(fileText, code);                // then main()
    strcat(fileText, methodsCreated);      // then function bodies

    free(code);
    return fileText;
}
