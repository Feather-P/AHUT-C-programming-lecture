#include <stdio.h>

int main(void) {
    int a[10];
    int i;
    int *p = a;

    for (i = 0; i < 10; i++) {
        scanf("%d", p + i);
    }

    for (i = 9; i >= 0; i--) {
        if (i < 9) {
            printf(" ");
        }
        printf("%d", *(p + i));
    }
    printf("\n");

    return 0;
}

