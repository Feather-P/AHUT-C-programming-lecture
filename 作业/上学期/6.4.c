#include <stdio.h>
int main() {
    double height = 100.0;
    double travel_length = 0;
    for (int i = 0; i < 10; i++) {
        travel_length += height;
        height /= 2;
        if (i < 9) {
            travel_length += height;
        }
    }
    printf("%lf %lf\n", travel_length, height);
    return 0;
}
