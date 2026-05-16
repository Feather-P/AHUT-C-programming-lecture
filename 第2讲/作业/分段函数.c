#include <math.h>
#include <stdio.h>

int main() {
    double input;
    scanf("%lf", &input);
    double output;
    if (input < 0) {
        output = pow(input, 3) + 2 * pow(input, 2) + input + 1;
    } else if (input > 0) {
        output = sqrt(input);
    } else {
        output = 3;
    }
    printf("%lf\n", output);
    return 0;
}