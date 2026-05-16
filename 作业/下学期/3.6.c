#include <stdio.h>

int main(void) {
    char str[5][15] = {"cccc", "ddd", "aa", "e", "bbbbb"};
    char tmp[15];
    int i, j, k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4 - i; j++) {
            k = 0;
            while (str[j][k] != '\0' && str[j + 1][k] != '\0' && str[j][k] == str[j + 1][k]) {
                k++;
            }

            if ((str[j][k] > str[j + 1][k]) ||
                (str[j][k] != '\0' && str[j + 1][k] == '\0')) {
                k = 0;
                while (str[j][k] != '\0') {
                    tmp[k] = str[j][k];
                    k++;
                }
                tmp[k] = '\0';

                k = 0;
                while (str[j + 1][k] != '\0') {
                    str[j][k] = str[j + 1][k];
                    k++;
                }
                str[j][k] = '\0';

                k = 0;
                while (tmp[k] != '\0') {
                    str[j + 1][k] = tmp[k];
                    k++;
                }
                str[j + 1][k] = '\0';
            }
        }
    }

    for (i = 0; i < 5; i++) {
        printf("%s\n", str[i]);
    }

    return 0;
}

