#include <stdio.h>

int main(void) {
    char ch;
    char s[81];
    int i = 0;
    int last = -1;

    scanf("%c", &ch);
    getchar();

    while ((s[i] = (char)getchar()) != '\n' && i < 80) {
        i++;
    }
    s[i] = '\0';

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == ch) {
            last = i;
        }
    }

    if (last == -1) {
        printf("Not Found\n");
    } else {
        printf("%d\n", last);
    }

    return 0;
}

