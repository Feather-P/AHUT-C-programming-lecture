#include <ctype.h>
#include <stdio.h>

int main() {
    char str[100];
    fgets(str, sizeof(str), stdin);
    for (int i = 0; i < 100; i++) {
        if (str[i] != '\0') {
            str[i] = (char)toupper(str[i]);
        } else {
            break;
        }
    }
    fputs(str, stdout);
}