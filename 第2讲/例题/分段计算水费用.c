#include <stdio.h>

int main() {
    double x;
    scanf("%lf", &x);
    double cost;
    if (x <= 15.0) {
        cost = 4 * x / 3.0;
    } else {
        cost = 2.5 * x - 10.5;
    }
    printf("%.2lf\n",cost);
}