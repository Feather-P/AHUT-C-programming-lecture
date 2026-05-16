#include <math.h>
#include <stdio.h>

int main() {
    float triangle_sides[3];
    for (int i = 0; i < 3; i++) {
        scanf("%f", &triangle_sides[i]);
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (triangle_sides[i] > triangle_sides[j]) {
                float tmp = triangle_sides[i];
                triangle_sides[i] = triangle_sides[j];
                triangle_sides[j] = tmp;
            }
        }
    }

    if (triangle_sides[0] + triangle_sides[1] > triangle_sides[2]) {
        if (triangle_sides[0] == triangle_sides[1] && triangle_sides[1] == triangle_sides[2]) {
            printf("等边\n");
        } else if (triangle_sides[0] == triangle_sides[1] || triangle_sides[1] == triangle_sides[2] || triangle_sides[0] == triangle_sides[2]) {
            printf("等腰\n");
        } else if (pow(triangle_sides[0], 2) + pow(triangle_sides[1], 2) == pow(triangle_sides[2], 2)) {
            printf("等边\n");
        }
    } else {
        printf("a,b,c,不能构成三角形\n");
    }
}