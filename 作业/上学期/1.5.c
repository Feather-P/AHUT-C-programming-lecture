#include <stdio.h>
int can_devide_by_3_and_5(int n) {
    if (n % 3 == 0 && n % 5 == 0) {
        return 1;
    }
    return 0;
}

int main() {
    int input;
    scanf("%d", &input);
    printf("%d\n", can_devide_by_3_and_5(input));
}