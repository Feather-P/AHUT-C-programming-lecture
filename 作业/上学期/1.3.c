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

int main() {
    int input;
    scanf("%d", &input);
    printf("%d\n", is_prime(input));
}