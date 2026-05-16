#include <stdio.h>
int main() {
    int input;
    scanf("%d", &input);
    switch (input) {
        case 1: {
            printf("selected 1.\n");
            break;
        }
        case 2: {
            printf("selected 2.\n");
            break;
        }
        default: {
            printf("select nothing.\n");
        }
    }
    return 0;
}