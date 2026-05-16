#include <stdio.h>

int main() {
    int num;
    const char INPUT = (char)getchar();
    if (INPUT >= 48 && INPUT <= 57) {
        num = INPUT - 48;
    } else if (INPUT >= 65 && INPUT <= 70) {
        num = INPUT - 55;
    } else if (INPUT >= 97 && INPUT <= 102){
        num = INPUT - 87;
    } else {
        printf("不合法的输入:%c\n", INPUT);
        return 1;
    }
    int ans[2];
    ans[1] = -1;
    ans[0] = num;
    if (num >= 7) {
        ans[0] = num / 7;
        ans[1] = num % 7;
    }
    printf("%d", ans[0]);
    if (ans[1] != -1) {
        printf("%d", ans[1]);
    }
    putchar('\n');
}