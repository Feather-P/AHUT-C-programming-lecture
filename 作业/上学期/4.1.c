#include <math.h>
#include <stdio.h>
int main() {
    double input;
    scanf("%lf", &input);
    double ans;
    if (input <= 0) {
        ans = 2*input - 1;
    } else if (0 < input && input < 15) {
        ans = sqrt(pow(input, 3)+ 14);
    } else {
        ans = pow(input, 0.4) + 3 * input + 1.0 / input;
    }
    printf("%.3lf\n", ans);
}