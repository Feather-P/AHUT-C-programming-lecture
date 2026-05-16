#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);

    if (n > 100) {
        int counter = 0;
        for (int i = 0; i <= 100; i++) {
            if (i % 5 == 0 || i % 3 == 0) {
                counter++;
                printf("%d", i);
                if (counter == 10) {
                    putchar('\n');
                    counter = 0;
                } else {
                    putchar(' ');
                }
            }
        }
    } else {
        int counter = 0;
        for (int i = 0; i <= n; i++) {
            if (i % 5 == 0 || i % 3 == 0) {
                counter++;
                printf("%d", i);
                if (counter == 10) {
                    putchar('\n');
                    counter = 0;
                } else {
                    putchar(' ');
                }
            }
        }
    }
    putchar('\n');
    return 0;
}