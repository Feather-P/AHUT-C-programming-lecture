#include <math.h>
#include <stdio.h>

double maxDouble(double a, double b) { return a > b ? a : b; }

int main() {
    double a;
    double b;
    double c;
    double d;
    double e;
    scanf("%lf %lf %lf %lf %lf", &a, &b, &c, &d, &e);
    double avg = (a + b + c + d + e) / 5;
    double m = maxDouble(a, maxDouble(b, maxDouble(c, maxDouble(d, e))));
    double std_drv = sqrt(0.2 * (pow(a - avg, 2) + pow(b - avg, 2) + pow(c - avg, 2) + pow(d - avg, 2) + pow(e - avg, 2)));
    printf("%lf %lf %lf\n", avg, m, std_drv);
    return 0;
}