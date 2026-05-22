#include <stdio.h>

int main(void) {
    char s[1000];
    FILE *fp;

    if (scanf("%999s", s) != 1) {
        return 0;
    }

    fp = fopen("f1.txt", "w");
    if (fp == NULL) {
        return 0;
    }

    fprintf(fp, "%s", s);
    fclose(fp);
    return 0;
}

