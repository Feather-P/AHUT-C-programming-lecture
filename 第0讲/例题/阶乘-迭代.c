#include <stdio.h>

int main() {
    int input;
    scanf("%d", &input);
    int tmp;
    int i = input - 1;
    tmp = input;
    while(i) {
        tmp = tmp *i;
        i--;
    }
    printf("Ans: %d\n",tmp);
    return 0;
}