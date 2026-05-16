#include <stdio.h>
#include <math.h>

int main() {
    double a, b, c;
    double s, area;
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        return 1;
    }
    s = (a + b + c) / 2.0;
    area = sqrt(s * (s - a) * (s - b) * (s - c));

    printf("%lf\n", area);

    return 0;
}