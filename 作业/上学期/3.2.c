#include <stdio.h>

struct Time {
    int hour;
    int minute;
    int second;
};

struct Time convertSecToTime(int seconds) {
    struct Time t;
    t.hour = seconds / 3600;
    seconds %= 3600;
    t.minute = seconds / 60;
    seconds %= 60;
    t.second = seconds / 60;
    return t;
}

int main() {
    int seconds;
    scanf("%d", &seconds);
    struct Time t = convertSecToTime(seconds);
    printf("%d:%d:%d\n", t.hour, t.minute, t.second);
    return 0;
}