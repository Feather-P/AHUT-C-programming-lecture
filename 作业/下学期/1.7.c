#include <stdio.h>

int main(void) {
    int n;
    int d[6];
    int i, j, t;
    int ans = 0;

    scanf("%d", &n);
    for (i = 5; i >= 0; i--) {
        d[i] = n % 10;
        n /= 10;
    }

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5 - i; j++) {
            if (d[j] < d[j + 1]) {
                t = d[j];
                d[j] = d[j + 1];
                d[j + 1] = t;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        ans = ans * 10 + d[i];
    }

    printf("%d\n", ans);

    return 0;
}

