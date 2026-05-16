#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool sameDigits(int num1, int num2) {
    int count[10] = {0};

    while (num1 > 0) {
        count[num1 % 10]++;
        num1 /= 10;
    }
    while (num2 > 0) {
        count[num2 % 10]--;
        num2 /= 10;
    }
    for (int i = 0; i < 10; i++) {
        if (count[i] != 0) {
            return false;
        }
    }
    return true;
}

bool isVampire(int num) {
    for (int i = 10; i <= 99; i++) {
        if (num % i == 0) {
            int j = num / i;
            if (j >= 10 && j <= 99) {
                if (sameDigits(num, (i * 100) + j)) {
                    if (i <= j) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    for (int num = 1000; num <= 9999; num++) {
        if (isVampire(num)) {
            printf("%d ", num);
        }
    }
    putchar('\n');
    return 0;
}
