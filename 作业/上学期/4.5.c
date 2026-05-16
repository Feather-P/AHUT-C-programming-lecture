#include <stdio.h>
#include <stdlib.h>
int main() {
    const int TARGET = (short int)((rand() % 100) + 1);
    int counter = 0;
    int guess_number = 0;
    while (guess_number != TARGET) {
        printf("请猜一个 1 到 100 之间的整数。\n请输入您的猜测:");
        scanf("%d", &guess_number);
        if (guess_number > TARGET) {
            printf("您猜的数字大于生成的数字\n");
        } else {
            printf("您猜的数字小于生成的数字\n");
        }
        counter++;
    }
    printf("猜对了，正确答案是: %d。您总共猜测了 %d 次。\n", TARGET, counter);
}