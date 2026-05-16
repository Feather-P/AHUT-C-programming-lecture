#include <stdio.h>

int prime(int n) {
    int i;
    if (n <= 1) {
        return 0;
    }
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int n;
    scanf("%d", &n);
    if (prime(n)) {
        printf("%d是素数\n", n);
    } else {
        printf("%d不是素数\n", n);
    }
    return 0;
}

