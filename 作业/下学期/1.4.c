#include <stdio.h>

int main(void) {
    int s[9];
    int i;
    int max, min;
    int sum = 0;

    for (i = 0; i < 9; i++) {
        scanf("%d", &s[i]);
    }

    max = s[0];
    min = s[0];
    for (i = 0; i < 9; i++) {
        if (s[i] > max) {
            max = s[i];
        }
        if (s[i] < min) {
            min = s[i];
        }
        sum += s[i];
    }

    printf("最后得分：%.2f\n", (sum - max - min) / 7.0);

    return 0;
}

