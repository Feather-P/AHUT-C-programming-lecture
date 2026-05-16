#include <stdio.h>

long long fac(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * fac(n - 1);
}

long long fun(int m, int n) {
    return fac(m) / (fac(n) * fac(m - n));
}

int main(void) {
    int m, n;
    scanf("%d%d", &m, &n);
    printf("%lld\n", fun(m, n));
    return 0;
}

