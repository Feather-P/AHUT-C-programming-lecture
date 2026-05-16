#include <stdio.h>

int main(void) {
    int a[4][4];
    int i, j, k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            int is_row_max = 1;
            int is_col_min = 1;
            for (k = 0; k < 4; k++) {
                if (a[i][k] > a[i][j]) {
                    is_row_max = 0;
                    break;
                }
            }
            for (k = 0; k < 4; k++) {
                if (a[k][j] < a[i][j]) {
                    is_col_min = 0;
                    break;
                }
            }
            if (is_row_max && is_col_min) {
                printf("%d\n", a[i][j]);
                return 0;
            }
        }
    }

    return 0;
}

