#include <stdio.h>

int cmp(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] > b[i]) {
            return 1;
        }
        if (a[i] < b[i]) {
            return -1;
        }
        i++;
    }
    if (a[i] == '\0' && b[i] == '\0') {
        return 0;
    }
    if (a[i] == '\0') {
        return -1;
    }
    return 1;
}

int main(void) {
    char a[256], b[256];
    int r;
    scanf("%s", a);
    scanf("%s", b);
    r = cmp(a, b);
    if (r < 0) {
        printf("%s<%s\n", a, b);
    } else if (r > 0) {
        printf("%s>%s\n", a, b);
    } else {
        printf("%s=%s\n", a, b);
    }
    return 0;
}

