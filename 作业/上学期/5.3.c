#include <math.h>
#include <stdio.h>
int main() {
    double weight;
    double height;
    scanf("%lf %lf",&weight,&height);
    double bmi = weight / pow(height, 2);
    if (bmi < 18.5) {
        printf("偏瘦");
    } else if (bmi < 24.0) {
        printf("正常");
    } else if (bmi < 28.0) {
        printf("过重");
    } else {
        printf("肥胖");
    }
    putchar('\n');
}