#include <stdio.h>

int is_prime(int n) {
    int i;
    if (n < 2) {
        return 0;
    }
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int collect_primes(int a[]) {
    int i;
    int cnt = 0;
    for (i = 2; i < 100; i++) {
        if (is_prime(i)) {
            a[cnt++] = i;
        }
    }
    return cnt;
}

int main(void) {
    int a[100];
    int i;
    int n = collect_primes(a);
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}

