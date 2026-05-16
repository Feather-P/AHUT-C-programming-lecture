#include <stdio.h>
int main() {
    int input;
    scanf("%d", &input);
    int output = 0;
    while (input != 0) {
        output *= 10;
        output += input % 10;
        input /= 10;
    }
    printf("%d\n", output);
}