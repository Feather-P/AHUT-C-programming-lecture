#include <stdio.h>

int main(void) {
    int n, x;
    int has_positive = 0;
    int first = 1;
    int i;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &x);
        if (x > 0) {
            if (!first) {
                printf(" ");
            }
            printf("%d", x);
            first = 0;
            has_positive = 1;
        }
    }

    if (!has_positive) {
        printf("NULL");
    }
    printf("\n");

    return 0;
}

