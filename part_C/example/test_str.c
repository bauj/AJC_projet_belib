#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void modify_str(char *teststr) {

    char str1[] = "1234567891011";
    teststr = (char * )realloc(teststr, strlen(str1) * sizeof(char));

    strcpy(teststr, str1);
}

int main() {
    char* teststr = (char *)malloc(sizeof(char));

    modify_str(teststr);

    printf("%s \n", teststr);

    free(teststr);
}