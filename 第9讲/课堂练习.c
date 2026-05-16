#include <stdio.h>

int main() {
    double ans = 0;
    double div = 1.0;  
    for (int i = 0; i < 100; i++) {
        if (i > 0) {
            div *= i;
        }
        ans += 1.0 / div;
    }
    printf("%.2lf\n", ans);
    return 0;
}