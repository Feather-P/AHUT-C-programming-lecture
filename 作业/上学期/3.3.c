#include <stdio.h>

int main() {
    double money_f;
    scanf("%lf", &money_f);
    int money = (int)(money_f * 1000);  // 转换为厘（0.001元）

    int c_100 = money / 100000;
    money %= 100000;

    int c_50 = money / 50000;
    money %= 50000;

    int c_20 = money / 20000;
    money %= 20000;

    int c_10 = money / 10000;
    money %= 10000;

    int c_5 = money / 5000;
    money %= 5000;

    int c_1 = money / 1000;
    money %= 1000;

    int c_0_5 = money / 500;
    money %= 500;

    int c_0_1 = money / 100;
    money %= 100;

    int c_0_0_5 = money / 50;
    money %= 50;

    int c_0_0_1 = money / 10;

    printf("%d %d %d %d %d %d %d %d %d %d\n", c_100, c_50, c_20, c_10, c_5, c_1, c_0_5, c_0_1, c_0_0_5, c_0_0_1);
    return 0;
}
