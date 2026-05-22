#include <stdio.h>
#include <string.h>

int find_name(char *name[], int n, const char *target) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(name[i], target) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    char *name[] = {"zhang", "wang", "sun", "chen", "huang"};
    char target[101];

    while (scanf("%100s", target) == 1) {
        if (find_name(name, 5, target)) {
            printf("Found\n");
        } else {
            printf("Not Found\n");
        }
    }

    return 0;
}

