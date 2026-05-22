#include <stdio.h>
#include <string.h>

void sort_strings(char *p[], int n) {
    int i, j;
    char *t;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (strcmp(p[j], p[j + 1]) > 0) {
                t = p[j];
                p[j] = p[j + 1];
                p[j + 1] = t;
            }
        }
    }
}

int main(void) {
    char s1[101], s2[101], s3[101], s4[101], s5[101];
    char *p[5] = {s1, s2, s3, s4, s5};
    int i;

    for (i = 0; i < 5; i++) {
        scanf("%100s", p[i]);
    }

    sort_strings(p, 5);
    for (i = 0; i < 5; i++) {
        printf("%s\n", p[i]);
    }

    return 0;
}

