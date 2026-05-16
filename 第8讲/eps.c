#include <math.h>
#include <stdio.h>
int main() {
    double input;
    scanf("%lf", &input);
    double value;
    value = 1;
    int div = 3;
    int flag = 0;
    while (fabs(value) >= input) {
        if (flag == 1) {
            value += 1.0 / div;
            div += 3;
            flag = 0;
        }
        else {
            value -= 1.0 / div;
            div += 3;
            flag = 1;
        }
    }
    printf("%lf\n", value);
    return 0;
}