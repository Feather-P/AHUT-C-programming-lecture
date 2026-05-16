#include <stdio.h>

int main() {
    int input;
    scanf("%d", &input);
    while (1) {
        if (input / 10 != 0) {
            printf("%d ", input % 10);
            input /= 10;
        } else {
            printf("%d\n", input);
            break;
        }
    }
}