#include <stdio.h>

int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i < num; ++i) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int is_odd(int num) {
    if (num % 2 == 1) {
        return 1;
    }
    return 0;
}

int main() {
    int sum1 = 0;
    int sum2 = 0;

    for (int i = 1; i <= 200; i++) {
        if (is_odd(i)) {
            sum1 += i;
        }

        if (is_prime(i)) {
            sum2 += i;
        }
    }

    printf("%d %d\n", sum1, sum2);
}