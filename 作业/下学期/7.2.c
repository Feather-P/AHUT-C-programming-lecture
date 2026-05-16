#include <stdio.h>

int main(void) {
    int a, b, c, t;
    int *p1 = &a, *p2 = &b, *p3 = &c;

    scanf("%d%d%d", &a, &b, &c);

    if (*p1 > *p2) {
        t = *p1;
        *p1 = *p2;
        *p2 = t;
    }
    if (*p1 > *p3) {
        t = *p1;
        *p1 = *p3;
        *p3 = t;
    }
    if (*p2 > *p3) {
        t = *p2;
        *p2 = *p3;
        *p3 = t;
    }

    printf("%d %d %d\n", *p1, *p2, *p3);
    return 0;
}

