#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>

void printStr(char *c){
    printf(c);
}

void printInt(int n){
    printf("%d", n);
}
void printBool(bool b){
    if (b){
        printf("true");
    }else{
        printf("false");
    }
}
void printNl(){
    printf("\n");
}