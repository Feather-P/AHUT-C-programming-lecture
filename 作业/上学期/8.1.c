#include <stdio.h>

int isSamePrefixAndSuffix(int num) {
    int a;
    int b;
    int c;
    int d;
    d = num % 10;
    num /= 10;
    c = num % 10;
    num /= 10;
    b = num % 10;
    num /= 10;
    a = num % 10;
    return a == b && c == d;
}

int main() {
    for (int i = 1; i < 100; i++) {
        int square = i * i;
        if (isSamePrefixAndSuffix(square)) {
            printf("%d\n", square);
        }
    } 
}

