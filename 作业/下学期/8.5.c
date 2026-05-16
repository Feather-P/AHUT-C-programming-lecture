#include <stdio.h>

int del_char(char *s, char ch) {
    int i = 0, j = 0;
    int found = 0;
    while (*(s + i) != '\0') {
        if (*(s + i) != ch) {
            *(s + j) = *(s + i);
            j++;
        } else {
            found = 1;
        }
        i++;
    }
    *(s + j) = '\0';
    return found;
}

int main(void) {
    char s[256];
    char ch;
    scanf("%s", s);
    scanf(" %c", &ch);

    if (del_char(s, ch)) {
        printf("%s\n", s);
    } else {
        printf("Not found\n");
    }

    return 0;
}

