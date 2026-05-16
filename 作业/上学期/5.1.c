#include <stdio.h>
int main() {
    unsigned int input;
    scanf("%d", &input);
    printf("星期");
    switch (input % 7) {
        case 1:
            printf("一");
            break;
        case 2:
            printf("二");
            break;
        case 3:
            printf("三");
            break;
        case 4:
            printf("四");
            break;
        case 5:
            printf("五");
            break;
        case 6:
            printf("六");
            break;
        case 0:
            printf("日");
            break;
    }
    putchar('\n');
    return 0;
}