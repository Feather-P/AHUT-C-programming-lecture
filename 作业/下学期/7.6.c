#include <stdio.h>

int main(void) {
    int a[3][4];
    int i, j;
    int maxv;
    int *p = &a[0][0];

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            scanf("%d", *(a + i) + j);
        }
    }

    maxv = *p;
    for (i = 0; i < 12; i++) {
        if (*(p + i) > maxv) {
            maxv = *(p + i);
        }
    }

    printf("%d\n", maxv);
    return 0;
}

