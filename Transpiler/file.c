#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "methods.h"
int add(int a,int b);
void main(){
int y =add(add( 1, 2), 2);
println( y);
}

int add(int a,int b){
return a + b;
}

