#include <stdio.h>

int main(void) {
    int a[6] = {6, 3, 8, 2, 5};
    int n = 5;
    int i, j, t;
    int x = 7;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }

    i = n - 1;
    while (i >= 0 && a[i] > x) {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = x;
    n++;

    for (i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
    printf("\n");

    return 0;
}

