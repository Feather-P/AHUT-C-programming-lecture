#include <stdio.h>

int main(void) {
    int n, m;
    int a[200], b[200];
    int i;
    int *pa = a;
    int *pb = b;

    scanf("%d%d", &n, &m);
    for (i = 0; i < n; i++) {
        scanf("%d", pa + i);
    }

    m %= n;
    for (i = 0; i < m; i++) {
        *(pb + i) = *(pa + (n - m + i));
    }
    for (i = m; i < n; i++) {
        *(pb + i) = *(pa + (i - m));
    }

    for (i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", *(pb + i));
    }
    printf("\n");

    return 0;
}

