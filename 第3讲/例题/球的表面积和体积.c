#include <math.h>
#include <stdio.h>

int main() {
    const double PI = 3.1415926;
    double radius = 0;
    printf("Enter the radius of the ball:");
    scanf("%lf", &radius);
    double volume = 0.75 * PI * pow(radius, 3.0);
    double facial_area = 4 * PI * pow(radius, 2);
    printf("Ball volume:%lf, facial area:%lf \n",volume , facial_area);
}