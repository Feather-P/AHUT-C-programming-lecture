#include <stdio.h>

int inverse(int x) {
    int sign = 1;
    int y = 0;
    if (x < 0) {
        sign = -1;
        x = -x;
    }
    while (x > 0) {
        y = y * 10 + x % 10;
        x /= 10;
    }
    return sign * y;
}

int main(void) {
    int x;
    scanf("%d", &x);
    printf("%d\n", inverse(x));
    return 0;
}

