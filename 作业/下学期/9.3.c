#include <stdio.h>
#include <string.h>

int delete_name(char name[][20], int n, const char *target) {
    int i, j;
    for (i = 0; i < n; i++) {
        if (strcmp(name[i], target) == 0) {
            for (j = i; j < n - 1; j++) {
                strcpy(name[j], name[j + 1]);
            }
            return n - 1;
        }
    }
    return n;
}

int main(void) {
    char name[][20] = {"zhang", "wang", "sun", "chen", "huang"};
    char target[21];
    int n = 5;
    int i;

    scanf("%20s", target);
    n = delete_name(name, n, target);

    for (i = 0; i < n; i++) {
        printf("%s\n", name[i]);
    }

    return 0;
}

