#include <stdio.h>

int cmp(char *a, char *b) {
    int i = 0;
    while (*(a + i) != '\0' && *(b + i) != '\0') {
        if (*(a + i) > *(b + i)) {
            return 1;
        }
        if (*(a + i) < *(b + i)) {
            return -1;
        }
        i++;
    }
    if (*(a + i) == '\0' && *(b + i) == '\0') {
        return 0;
    }
    if (*(a + i) == '\0') {
        return -1;
    }
    return 1;
}

void sort3(char *p[]) {
    int i, j;
    char *t;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2 - i; j++) {
            if (cmp(p[j], p[j + 1]) > 0) {
                t = p[j];
                p[j] = p[j + 1];
                p[j + 1] = t;
            }
        }
    }
}

int main(void) {
    char s1[101], s2[101], s3[101];
    char *p[3] = {s1, s2, s3};
    int i;

    scanf("%s", s1);
    scanf("%s", s2);
    scanf("%s", s3);

    sort3(p);
    for (i = 0; i < 3; i++) {
        printf("%s\n", p[i]);
    }

    return 0;
}

