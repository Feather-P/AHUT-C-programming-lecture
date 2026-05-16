#include <stdio.h>

int main() {
    const char INPUT = (char)getchar();
    if (65 <= INPUT && INPUT <= 90) {
        printf("%c ", '1');
    }else if (97 <= INPUT && INPUT <= 122) {
        printf("%c ", '0');
    }else {
        printf("[Not a alphabet] ");
    }

    printf("%o\n", INPUT);
}