#include <ctype.h>
#include <stdio.h>

int main() {
    char c;
    c = getchar();
    if (isdigit(c)) {
        printf("%c\n", c);
    } else {
        printf("%d\n", c - 55);
    }
}