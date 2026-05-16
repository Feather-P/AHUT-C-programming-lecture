#include <stdio.h>

int primes[100];
int cnt = 0;

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

void collect_primes(void) {
    int i;
    cnt = 0;
    for (i = 2; i < 100; i++) {
        if (is_prime(i)) {
            primes[cnt++] = i;
        }
    }
}

int main(void) {
    int i;
    collect_primes();
    for (i = 0; i < cnt; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");
    return 0;
}

