#include <stdio.h>

void to_str_recursive(int n) {
    if (n >= 10) {
        to_str_recursive(n / 10);
    }
    putchar('0' + n % 10);
}

