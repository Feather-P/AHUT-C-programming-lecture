#include <stdio.h>

typedef struct {
    int id;
    char name[32];
    int score;
} Student;

void insertion_sort_desc(Student s[], int n) {
    int i, j;
    Student key;
    for (i = 1; i < n; i++) {
        key = s[i];
        j = i - 1;
        while (j >= 0 && s[j].score < key.score) {
            s[j + 1] = s[j];
            j--;
        }
        s[j + 1] = key;
    }
}

int main(void) {
    Student s[5];
    int i;
    int fail = 0;

    for (i = 0; i < 5; i++) {
        scanf("%d %31s %d", &s[i].id, s[i].name, &s[i].score);
    }

    insertion_sort_desc(s, 5);

    for (i = 0; i < 5; i++) {
        if (s[i].score < 60) {
            printf("%s %d\n", s[i].name, s[i].score);
            fail = 1;
        }
    }

    if (!fail) {
        printf("没有不及格的学生\n");
    }

    return 0;
}

