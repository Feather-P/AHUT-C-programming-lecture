#include <stdio.h>

int main(void) {
    int n;
    char s[201];
    char t[201];
    int i, len = 0;

    scanf("%d", &n);
    scanf("%s", s);

    while (s[len] != '\0') {
        len++;
    }

    for (i = 0; i < len; i++) {
        t[i] = s[(i + n) % len];
    }
    t[len] = '\0';

    printf("%s\n", t);

    return 0;
}

