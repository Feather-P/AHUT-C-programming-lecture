#include <stdio.h>

int factorial(int n) {
    if (n == 0) return 1;
    return n * factorial(n-1);
}

int main() {
    int input;
    scanf("%d", &input);
    printf("Ans: %d \n", factorial(input));
}