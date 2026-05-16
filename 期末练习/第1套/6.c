#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int magic = rand() % 10;
    int input;
    scanf("%d", &input);
    if (input == magic) {
        printf("正确\n");
    } else {
        printf("错误，");
        if (input > magic) {
            printf("大了\n");
        } else {
            printf("小了\n");
        }
    }
}