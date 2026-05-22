#include <stdio.h>
#include <string.h>

typedef union {
    char student_id[16];
    char teacher_id[16];
} Number;

typedef struct {
    char name[32];
    char gender[16];
    int age;
    char identity[16];
    Number num;
} Person;

int main(void) {
    Person p[3];
    int i;

    for (i = 0; i < 3; i++) {
        scanf("%31s %15s %d %15s", p[i].name, p[i].gender, &p[i].age, p[i].identity);
        if (strcmp(p[i].identity, "student") == 0) {
            scanf("%15s", p[i].num.student_id);
        } else {
            scanf("%15s", p[i].num.teacher_id);
        }
    }

    for (i = 0; i < 3; i++) {
        if (strcmp(p[i].identity, "student") == 0) {
            printf("%s %s %d %s %s\n", p[i].name, p[i].gender, p[i].age, p[i].identity, p[i].num.student_id);
        } else {
            printf("%s %s %d %s %s\n", p[i].name, p[i].gender, p[i].age, p[i].identity, p[i].num.teacher_id);
        }
    }

    return 0;
}

