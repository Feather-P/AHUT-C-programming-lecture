#include <stdio.h>

void fun(char s[]) {
    int i = 0;
    int j;
    char t;
    while (s[i] != '\0') {
        i++;
    }
    j = i - 1;
    i = 0;
    while (i < j) {
        t = s[i];
        s[i] = s[j];
        s[j] = t;
        i++;
        j--;
    }
}

int main(void) {
    char s[256];
    scanf("%s", s);
    fun(s);
    printf("%s\n", s);
    return 0;
}

