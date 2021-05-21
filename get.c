#include <string.h>
#include "get.h"
#include <stdlib.h>
#include <stdio.h>

int Get_Int(int *a) {
    int n = 0;
    while (n == 0) {
        n = scanf("%d", a);
        if (n == 0) {

            printf("Error reading integer value\n");
            scanf("%*[^\n]");
            n = 0;
        }
    }
    return n < 0 ? 0 : 1;
}
int Get_Int0(int *a) {
    int n = 0;
    while (n == 0) {
        n = scanf("%d", a);
        if (n == 0 || *a <= 0) {
            printf("Error reading positive integer value\n");
            scanf("%*[^\n]");
            n = 0;
        }
    }
    return n < 0 ? 0 : 1;
}

char *Get_Str() {
    int n;
    char *res;
    n = scanf(" %m[^\n]", &res);
    if (n == -1) {
        free(res);
        return NULL;
    }
    return res;
}