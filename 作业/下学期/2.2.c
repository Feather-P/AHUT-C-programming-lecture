#include <stdio.h>

int main(void) {
    int a[3][4];
    int i, j;
    int pos = 0, neg = 0, zero = 0;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            scanf("%d", &a[i][j]);
            if (a[i][j] > 0) {
                pos++;
            } else if (a[i][j] < 0) {
                neg++;
            } else {
                zero++;
            }
        }
    }

    printf("正数：%d个\n", pos);
    printf("负数：%d个\n", neg);
    printf("零：%d个\n", zero);

    return 0;
}

