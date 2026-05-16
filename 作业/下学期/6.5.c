#include <stdio.h>

void mcopy(char s[], char t[], int m) {
    int i = 0;
    int j = m - 1;
    while (s[j] != '\0') {
        t[i++] = s[j++];
    }
    t[i] = '\0';
}

int main(void) {
    char s[256], t[256];
    int m;
    scanf("%s", s);
    scanf("%d", &m);
    mcopy(s, t, m);
    printf("%s\n", t);
    return 0;
}
