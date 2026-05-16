#include <stdio.h>
int main() {
    int input[3];
    // 读入
    for (int i = 0; i < 3; i++) {
        scanf("%d", &input[i]);
    }

    int min = input[0];
    int max = input[0];
    for (int i = 1; i < 3; i++) {
        if (min > input[i]) {
            min = input[i];
        }
        if (max < input[i]) {
            max = input[i];
        }
    }

    printf("%d %d\n", max, min);

    return 0;
}