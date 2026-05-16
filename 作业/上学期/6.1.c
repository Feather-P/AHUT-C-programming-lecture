#include <stdio.h>
int gcd(int a, int b) {
    while (b != 0) {
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

int lcm (int a, int b) {
    return (a * b) / gcd(a, b);
}

int main() {
    int a, b;
    scanf("%d %d", &a, &b);
    printf("%d %d\n", gcd(a, b), lcm(a, b));
    return 0;
}