#include <stdio.h>

int main(void) {
    char s1[201], s2[201], out[402];
    int i = 0, j = 0;
    int cmp = 0;
    int p = 0;

    scanf("%s", s1);
    scanf("%s", s2);

    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] > s2[i]) {
            cmp = 1;
            break;
        }
        if (s1[i] < s2[i]) {
            cmp = -1;
            break;
        }
        i++;
    }
    if (cmp == 0) {
        if (s1[i] == '\0' && s2[i] != '\0') {
            cmp = -1;
        } else if (s1[i] != '\0' && s2[i] == '\0') {
            cmp = 1;
        }
    }

    if (cmp >= 0) {
        i = 0;
        while (s1[i] != '\0') {
            out[p++] = s1[i++];
        }
        while (s2[j] != '\0') {
            out[p++] = s2[j++];
        }
    } else {
        i = 0;
        while (s2[i] != '\0') {
            out[p++] = s2[i++];
        }
        while (s1[j] != '\0') {
            out[p++] = s1[j++];
        }
    }
    out[p] = '\0';

    printf("%s\n", out);

    return 0;
}

