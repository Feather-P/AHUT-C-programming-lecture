#include <stdio.h>

int is_perfect(int n) {
    int i;
    int sum = 0;
    if (n <= 1) {
        return 0;
    }
    for (i = 1; i <= n / 2; i++) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum == n;
}

int main(void) {
    int n;
    scanf("%d", &n);
    if (is_perfect(n)) {
        printf("是\n");
    } else {
        printf("否\n");
    }
    return 0;
}

