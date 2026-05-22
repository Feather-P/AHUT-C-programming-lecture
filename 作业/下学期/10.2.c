#include <stdio.h>

typedef struct {
    int id;
    char name[32];
    int total;
} Student;

void stats(Student *s, int n, double *avg, int *fail_cnt) {
    int i;
    int sum = 0;
    int fail = 0;
    for (i = 0; i < n; i++) {
        sum += (s + i)->total;
        if ((s + i)->total < 60) {
            fail++;
        }
    }
    *avg = (double)sum / n;
    *fail_cnt = fail;
}

int main(void) {
    Student s[5];
    int i;
    double avg;
    int fail_cnt;

    for (i = 0; i < 5; i++) {
        scanf("%d %31s %d", &s[i].id, s[i].name, &s[i].total);
    }

    stats(s, 5, &avg, &fail_cnt);
    printf("平均分：%.2f\n", avg);
    printf("不及格人数：%d\n", fail_cnt);

    return 0;
}

