#include <stdio.h>
int main() {
    int input;
    scanf("%d", &input);

    int ans = !(input % 2);
    printf("%d\n", ans);

    return 0;
}