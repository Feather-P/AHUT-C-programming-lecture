#include <ctype.h>
#include <stdio.h>
#include <string.h>
int main() {
    char input[100];
    scanf("%s", input);
    int idx = 0;
    char now = input[0];

    int lower_count = 0;
    int upper_count = 0;
    int digit_count = 0;
    int other_count = 0;

    char target_lower[100];
    char target_upper[100];
    while (now != '\0') {
        if (islower(now)) {
            lower_count++;
            int len = strlen(target_lower);
            target_lower[len] = now;
            target_lower[len+1] = '\0';
        } else if (isupper(now)) {
            upper_count++;
            int len = strlen(target_upper);
            target_upper[len] = now;
            target_upper[len+1] = '\0';
        } else if (isdigit(now)){
            digit_count++;
        } else {
            other_count++;
        }
        idx++;
        now = input[idx];
    }
    printf("小写:%d\n大写:%d\n数字:%d\n其他:%d\n小写字符串:%s大写字符串:%s", lower_count, upper_count,digit_count,other_count,target_lower,target_upper);
    return 0;
}