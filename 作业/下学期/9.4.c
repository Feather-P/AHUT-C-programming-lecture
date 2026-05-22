#include <stdio.h>
#include <string.h>

int subString(const char *str, const char *sub) {
    int cnt = 0;
    int i, len_sub;

    len_sub = (int)strlen(sub);
    if (len_sub == 0) {
        return 0;
    }

    for (i = 0; str[i] != '\0'; i++) {
        if (strncmp(str + i, sub, len_sub) == 0) {
            cnt++;
        }
    }
    return cnt;
}

int main(void) {
    char str[1001];
    char sub[1001];

    scanf("%1000s", str);
    scanf("%1000s", sub);

    printf("%d\n", subString(str, sub));
    return 0;
}

