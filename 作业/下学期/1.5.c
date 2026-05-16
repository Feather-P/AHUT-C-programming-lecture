#include <stdio.h>

int main(void) {
    int cnt[5] = {0};
    int x;
    int i;

    while (1) {
        scanf("%d", &x);
        if (x == -1) {
            break;
        }
        if (x >= 0 && x <= 4) {
            cnt[x]++;
        }
    }

    for (i = 0; i < 5; i++) {
        printf("%d：%d个\n", i, cnt[i]);
    }

    return 0;
}

