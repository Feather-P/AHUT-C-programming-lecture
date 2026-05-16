#include <stdio.h>

int max_rec(int a[], int n) {
    int t;
    if (n == 1) {
        return a[0];
    }
    t = max_rec(a, n - 1);
    return a[n - 1] > t ? a[n - 1] : t;
}

int main(void) {
    int n;
    int a[100];
    int i;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    printf("%d\n", max_rec(a, n));

    return 0;
}

