#include <stdio.h>

int main(void) {
    int a[3][3];
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    for (i = 0; i < 3; i++) {
        int d = a[i][i];
        for (j = 0; j < 3; j++) {
            a[i][j] = a[i][j] / d;
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (j > 0) {
                printf(" ");
            }
            printf("%d", a[i][j]);
        }
        printf("\n");
    }

    return 0;
}

