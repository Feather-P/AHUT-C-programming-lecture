#include <stdio.h>

int isContainSeven(int num) {
    while (num != 0) {
        int digit = num % 10;
        if (digit == 7) {
            return 1;
        }
        num /= 10;
    }
    return 0;
}

int canDivByThree(int num) {
    return num % 3 == 0;
}

int main() {
    int sum = 0;
    for (int i = 0; i <= 300; i++) {
        if (isContainSeven(i) && canDivByThree(i)) {
            sum += i;
        }
    }
    printf("%d\n", sum);
}