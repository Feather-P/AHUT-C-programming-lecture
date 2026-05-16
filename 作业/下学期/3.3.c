#include <stdio.h>

int main(void) {
    char s[201];
    int i, len = 0;
    int ok = 1;

    scanf("%s", s);
    while (s[len] != '\0') {
        len++;
    }

    for (i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) {
            ok = 0;
            break;
        }
    }

    if (ok) {
        printf("是回文\n");
    } else {
        printf("不是回文\n");
    }

    return 0;
}

