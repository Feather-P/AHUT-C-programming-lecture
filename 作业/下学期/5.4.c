#include <stdio.h>

int fun(int n) {
    if (n < 0) {
        n = -n;
    }
    if (n < 10) {
        return n;
    }
    return n % 10 + fun(n / 10);
}

int main(void) {
    int n;
    scanf("%d", &n);
    printf("%d\n", fun(n));
    return 0;
}

