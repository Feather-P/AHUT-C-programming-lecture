#include <stdio.h>

void sort(int *a, int n) {
    int i, j, k, t;
    for (i = 0; i < n - 1; i++) {
        k = i;
        for (j = i + 1; j < n; j++) {
            if (*(a + j) < *(a + k)) {
                k = j;
            }
        }
        if (k != i) {
            t = *(a + i);
            *(a + i) = *(a + k);
            *(a + k) = t;
        }
    }
}

int main(void) {
    int n;
    int a[200];
    int i;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    sort(a, n);
    for (i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
    printf("\n");

    return 0;
}

