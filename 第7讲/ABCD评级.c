#include <stdio.h>
int main() {
    int score;
    scanf("%d", &score);
    switch (score / 10) {
        case 10: printf("A\n"); break;
        case 9: printf("A\n"); break;
        case 8: printf("B\n"); break;
        case 7: printf("C\n"); break;
        default: printf("D\n");
    }
    return 0;
}