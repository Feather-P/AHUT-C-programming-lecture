#include <stdio.h>

const double EPSILON = 1e-5;

int main() {
    double num;
    scanf("%lf", &num);
    double left = 0;
    double right = num;
    double ans = -1;
    while (right - left > EPSILON) {
        double mid = (left + ((right - left) / 2));
        if (mid*mid < num) {
            left = mid;
        } else {
            right = mid;
        }
        ans = mid;
    }
    printf("%.4lf\n",ans);
}