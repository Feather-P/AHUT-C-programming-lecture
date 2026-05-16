#include <stdio.h>

void find_max_min(int a[], int n, int *maxv, int *minv) {
    int i;
    *maxv = a[0];
    *minv = a[0];
    for (i = 1; i < n; i++) {
        if (a[i] > *maxv) {
            *maxv = a[i];
        }
        if (a[i] < *minv) {
            *minv = a[i];
        }
    }
}

int main(void) {
    int n, i;
    int a[200];
    int maxv, minv;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    find_max_min(a, n, &maxv, &minv);
    printf("最大值：%d\n", maxv);
    printf("最小值：%d\n", minv);

    return 0;
}

