#include <stdio.h>

int main(void) {
    int n;
    int a[6][6];
    int i, j;
    int sum_main = 0;
    int prod_sec = 1;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    for (i = 0; i < n; i++) {
        sum_main += a[i][i];
        prod_sec *= a[i][n - 1 - i];
    }

    printf("主对角线之和：%d\n", sum_main);
    printf("辅对角线之积：%d\n", prod_sec);

    return 0;
}

