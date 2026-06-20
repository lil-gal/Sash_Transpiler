#define print(x) _Generic((x), \
    char*: printStr,           \
    int:   printInt,           \
    bool:  printBool           \
)(x)

#define println(x) do {print(x); printNl();} while(0)

void printStr(char *c);
void printInt(int n);
void printBool(bool b);
void printNl();