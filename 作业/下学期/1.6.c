#include <stdio.h>

int main(void) {
    int m, n;
    int a[100], b[100], c[100];
    int i;
    int len;

    scanf("%d%d", &m, &n);
    for (i = 0; i < m; i++) {
        scanf("%d", &a[i]);
    }
    for (i = 0; i < n; i++) {
        scanf("%d", &b[i]);
    }

    len = (m > n) ? m : n;
    for (i = 0; i < len; i++) {
        int ai = 0;
        int bi = 0;
        if (i >= len - m) {
            ai = a[i - (len - m)];
        }
        if (i >= len - n) {
            bi = b[i - (len - n)];
        }
        c[i] = ai + bi;
    }

    for (i = 0; i < len; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", c[i]);
    }
    printf("\n");

    return 0;
}

