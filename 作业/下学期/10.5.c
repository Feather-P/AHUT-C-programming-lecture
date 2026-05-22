#include <stdio.h>

typedef struct {
    int id;
    char name[32];
    int c1, c2, c3;
    int sum;
} Student;

int main(void) {
    Student s[5], t;
    int i, j;

    for (i = 0; i < 5; i++) {
        scanf("%d %31s %d %d %d", &s[i].id, s[i].name, &s[i].c1, &s[i].c2, &s[i].c3);
        s[i].sum = s[i].c1 + s[i].c2 + s[i].c3;
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4 - i; j++) {
            if (s[j].sum < s[j + 1].sum) {
                t = s[j];
                s[j] = s[j + 1];
                s[j + 1] = t;
            }
        }
    }

    for (i = 0; i < 5; i++) {
        printf("%d %s %d %d %d\n", s[i].id, s[i].name, s[i].c1, s[i].c2, s[i].c3);
    }

    return 0;
}

