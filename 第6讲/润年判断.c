#include <stdio.h>
#include <stdlib.h>

int main() {
    int* year = malloc(sizeof(int));
    scanf("%d",year);
    if ((*year % 4 == 0 && *year % 100 != 0) || *year % 400 == 0) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    free(year);
    return 0;
}