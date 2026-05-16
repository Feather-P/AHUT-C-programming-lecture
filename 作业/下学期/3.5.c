#include <stdio.h>

int main(void) {
    char s[201];
    int i = 0;
    int first = 1;

    scanf("%s", s);

    while (s[i] != '\0') {
        int v;
        if (s[i + 1] != '\0') {
            v = (s[i] - '0') * 10 + (s[i + 1] - '0');
            i += 2;
        } else {
            v = s[i] - '0';
            i += 1;
        }

        if (!first) {
            printf(",");
        }
        printf("%d", v);
        first = 0;
    }
    printf("\n");

    return 0;
}

