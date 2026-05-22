#include <stdio.h>

typedef struct {
    int id;
    char name[32];
    int score;
} Student;

int main(void) {
    FILE *fa = fopen("a.txt", "r");
    FILE *fb;
    Student cur, best;
    int has_data = 0;

    if (fa == NULL) {
        return 0;
    }

    while (fscanf(fa, "%d %31s %d", &cur.id, cur.name, &cur.score) == 3) {
        if (!has_data || cur.score > best.score) {
            best = cur;
            has_data = 1;
        }
    }
    fclose(fa);

    fb = fopen("b.txt", "w");
    if (fb == NULL) {
        return 0;
    }
    if (has_data) {
        fprintf(fb, "%d %s %d", best.id, best.name, best.score);
    }
    fclose(fb);

    return 0;
}

