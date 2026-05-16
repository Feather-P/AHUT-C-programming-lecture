#include <stdio.h>

int check(char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        return 1;
    }
    if (ch >= '0' && ch <= '9') {
        return 2;
    }
    return 3;
}

int main(void) {
    char s[256];
    int i = 0;
    int letter = 0, digit = 0, other = 0;

    while ((s[i] = (char)getchar()) != '\n' && s[i] != '\0' && i < 255) {
        i++;
    }
    s[i] = '\0';

    for (i = 0; s[i] != '\0'; i++) {
        int t = check(s[i]);
        if (t == 1) {
            letter++;
        } else if (t == 2) {
            digit++;
        } else {
            other++;
        }
    }

    printf("字母：%d\n", letter);
    printf("数字：%d\n", digit);
    printf("其它字符：%d\n", other);
    return 0;
}

