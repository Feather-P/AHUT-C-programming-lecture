#include <stdio.h>

int main(void) {
    int n;
    int a[10];
    int i;
    double sum = 0.0;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        sum += a[i];
    }

    printf("平均值为：%.2f\n", sum / n);
    printf("逆序输出的结果是：");
    for (i = n - 1; i >= 0; i--) {
        if (i != n - 1) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
    printf("\n");

    return 0;
}

