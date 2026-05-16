#include <stdio.h>

int main() {
    int day;
    scanf("%d", &day);
    
    if (day <= 31) {
        printf("1月%d日\n", day);
    } else if (day <= 59) {
        printf("2月%d日\n", day - 31);
    } else if (day <= 90) {
        printf("3月%d日\n", day - 59);
    } else if (day <= 120) {
        printf("4月%d日\n", day - 90);
    } else if (day <= 151) {
        printf("5月%d日\n", day - 120);
    } else if (day <= 181) {
        printf("6月%d日\n", day - 151);
    } else if (day <= 212) {
        printf("7月%d日\n", day - 181);
    } else if (day <= 243) {
        printf("8月%d日\n", day - 212);
    } else if (day <= 273) {
        printf("9月%d日\n", day - 243);
    } else if (day <= 304) {
        printf("10月%d日\n", day - 273);
    } else if (day <= 334) {
        printf("11月%d日\n", day - 304);
    } else {
        printf("12月%d日\n", day - 334);
    }
    
    return 0;
}