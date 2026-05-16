#include <stdio.h>

int factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    for (int i = 100; i < 1000; i++) {
        int num = i;
        int sum = 0;
        while (num != 0) {
            sum += factorial(num % 10);
            num /= 10;
        }
        if (sum == i) {
            printf("%d\n", i);
        }
    }
}