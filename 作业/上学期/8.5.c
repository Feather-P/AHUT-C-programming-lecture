#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculate_sum_and_print() {
    char s1[100], s2[100];
    long long num1, num2, sum;
    char result_str[100];

    scanf("%s", s1);

    scanf("%s", s2);
    num1 = atoll(s1);
    num2 = atoll(s2);
    sum = num1 + num2;
    sprintf(result_str, "%lld", sum);

    printf("%s\n", result_str);
}

int main() {
    calculate_sum_and_print();
    return 0;
}