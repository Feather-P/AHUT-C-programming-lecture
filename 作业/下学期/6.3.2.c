#include <stdio.h>

void to_str_recursive(int n);

int main(void) {
    int n;
    scanf("%d", &n);
    if (n < 0) {
        putchar('-');
        n = -n;
    }
    to_str_recursive(n);
    putchar('\n');
    return 0;
}

