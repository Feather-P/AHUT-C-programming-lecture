#include <stdio.h>

int steps(long long n) {
    if (n == 1) {
        return 0;
    }
    if (n % 2 == 0) {
        return 1 + steps(n / 2);
    }
    return 1 + steps(3 * n + 1);
}

int main(void) {
    long long n;
    scanf("%lld", &n);
    printf("%d\n", steps(n));
    return 0;
}

