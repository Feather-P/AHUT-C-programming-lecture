#include <stdio.h>

int main(void) {
    char *week[] = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };
    int n;

    scanf("%d", &n);
    if (n >= 1 && n <= 7) {
        printf("%s\n", week[n - 1]);
    } else {
        printf("输入数据不合法\n");
    }

    return 0;
}

