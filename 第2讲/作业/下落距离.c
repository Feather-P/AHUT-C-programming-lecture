#include <stdio.h>

const float g = 9.8;

int main() {
    const int DURATION = 10;
    float distance = g * DURATION * DURATION / 2.0;
    printf("%f\n", distance);
    return 0;
}