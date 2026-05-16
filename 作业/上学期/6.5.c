#include <stdio.h>
int main() {
    int target;
    scanf("%d", &target);
    int baka_count = 0; // ⑨!
    for (int i = 1; i <= target; i++) {
        int num = i;
        while (num != 0) {
            if (num % 10 == 9) {
                baka_count++;
            }
            num /= 10;
        }
    }
    printf("%d\n", baka_count);
    return 0;
}