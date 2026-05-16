#include <stdio.h>

int last_person(int n) {
    int a[1000];
    int i;
    int left = n;
    int p = 0;
    int cnt = 0;

    for (i = 0; i < n; i++) {
        a[i] = i + 1;
    }

    while (left > 1) {
        if (a[p] != 0) {
            cnt++;
            if (cnt == 3) {
                a[p] = 0;
                cnt = 0;
                left--;
            }
        }
        p = (p + 1) % n;
    }

    for (i = 0; i < n; i++) {
        if (a[i] != 0) {
            return a[i];
        }
    }
    return -1;
}

int main(void) {
    int n;
    scanf("%d", &n);
    printf("%d\n", last_person(n));
    return 0;
}

