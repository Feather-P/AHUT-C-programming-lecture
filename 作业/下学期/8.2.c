#include <stdio.h>

float *get_student_scores(float (*a)[4], int idx) {
    return *(a + idx);
}

int main(void) {
    float score[3][4];
    int k;
    int i, j;
    float *p;

    scanf("%d", &k);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            scanf("%f", *(score + i) + j);
        }
    }

    p = get_student_scores(score, k - 1);
    for (i = 0; i < 4; i++) {
        printf("%.1f ", *(p + i));
    }
    printf("\n");

    return 0;
}

