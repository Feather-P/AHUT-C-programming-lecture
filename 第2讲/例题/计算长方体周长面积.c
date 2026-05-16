#include <stdio.h>
const float PI = 3.14;

int main() {
    int a = 3;
    int b = 4;
    int perimeter1 = 2 * (a+b);
    int square1 = a*b;

    int radius = 3;
    float perimeter2 = 2 * PI * radius;
    float square2 = PI * radius * radius;

    printf("正方体周长:%d\n正方体面积:%d\n圆形周长:%f\n圆形面积:%f\n",perimeter1,square1,perimeter2,square2);
}