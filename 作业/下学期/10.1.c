#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[32];
    int total;
} Student;

int main(void) {
    Student s[5];
    int target;
    int i, found = -1;

    for (i = 0; i < 5; i++) {
        scanf("%d %31s %d", &s[i].id, s[i].name, &s[i].total);
    }
    scanf("%d", &target);

    for (i = 0; i < 5; i++) {
        if (s[i].id == target) {
            found = i;
            break;
        }
    }

    if (found >= 0) {
        printf("%s %d\n", s[found].name, s[found].total);
    } else {
        printf("Not Found!\n");
    }

    return 0;
}

