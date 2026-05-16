#include <stdio.h>

int main(void) {
    int a, b, c;
    int *pmax;

    scanf("%d%d%d", &a, &b, &c);

    pmax = &a;
    if (b > *pmax) {
        pmax = &b;
    }
    if (c > *pmax) {
        pmax = &c;
    }

    printf("%d\n", *pmax);
    return 0;
}

