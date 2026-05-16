#include <stdio.h>

int main() {
    const double PI = 3.14159;

    double radius;
    double height;
    scanf("%lf %lf",&radius, &height);

    printf("%.6lf ", 2 * radius * PI);
    printf("%.6lf ", 2 * radius * PI * height);
    printf("%.6lf ", PI * radius * radius);
    printf("%.6lf\n", PI * radius * radius * height);
}