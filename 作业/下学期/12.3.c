#include <stdio.h>

int main(void) {
    FILE *fa = fopen("a.txt", "r");
    FILE *fc;
    int id, score;
    char name[32];
    int cnt = 0;
    double sum = 0.0;

    if (fa == NULL) {
        return 0;
    }

    while (fscanf(fa, "%d %31s %d", &id, name, &score) == 3) {
        sum += score;
        cnt++;
    }
    fclose(fa);

    fc = fopen("c.txt", "w");
    if (fc == NULL) {
        return 0;
    }

    if (cnt > 0) {
        fprintf(fc, "%.2f", sum / cnt);
    }
    fclose(fc);
    return 0;
}

