#include <stdio.h>

float Area(float r) {
    return 3.14f * r * r;
}

int main(void) {
    float r1, r2;
    scanf("%f%f", &r1, &r2);
    printf("%.2f\n", Area(r1) - Area(r2));
    return 0;
}

