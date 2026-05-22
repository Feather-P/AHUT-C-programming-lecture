#include <stdio.h>

typedef struct {
    int id;
    char name[32];
    int c1, c2, c3;
} Student;

int main(void) {
    Student s[5];
    int i;

    for (i = 0; i < 5; i++) {
        scanf("%d %31s %d %d %d", &s[i].id, s[i].name, &s[i].c1, &s[i].c2, &s[i].c3);
    }

    for (i = 0; i < 5; i++) {
        printf("%d %s %d %d %d\n", s[i].id, s[i].name, s[i].c1, s[i].c2, s[i].c3);
    }

    return 0;
}

