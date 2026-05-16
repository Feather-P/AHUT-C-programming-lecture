#include <stdio.h>

long long fac(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * fac(n - 1);
}

int main(void) {
    int n;
    scanf("%d", &n);
    printf("%lld\n", fac(n));
    return 0;
}

