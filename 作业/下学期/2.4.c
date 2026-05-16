#include <stdio.h>

int main(void) {
    int a[4][4];
    int i, j;
    int maxv;
    int maxi = 0, maxj = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    maxv = a[0][0];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (a[i][j] > maxv) {
                maxv = a[i][j];
                maxi = i;
                maxj = j;
            }
        }
    }

    printf("最大元素：%d\n", maxv);
    printf("下标：%d,%d\n", maxi, maxj);

    return 0;
}

