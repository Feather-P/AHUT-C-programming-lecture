#include <math.h>
#include <stdio.h>
int main() {
    int x;
    scanf("%d", &x);
    double y;
    if (x <= 0) {
        y = -2.5 * x - 36;
    } else if (x > 0 && x < 10) {
        y = 1.0 / x;
    } else if(x >= 10 && x <= 15){
        y = sqrt(pow(x, 2) + pow(x - 1.5, 3));
    } else {
        y = pow(0.7, x) + 16.3;
    }
    printf("%lf\n", y);
}