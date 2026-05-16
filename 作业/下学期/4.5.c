#include <stdio.h>

void factorize(int n) {
    int x = n;
    int i;
    int first = 1;
    printf("%d=", n);
    for (i = 2; i <= x; i++) {
        while (x % i == 0) {
            if (!first) {
                printf("*");
            }
            printf("%d", i);
            first = 0;
            x /= i;
        }
    }
    printf("\n");
}

int main(void) {
    int n;
    scanf("%d", &n);
    factorize(n);
    return 0;
}

