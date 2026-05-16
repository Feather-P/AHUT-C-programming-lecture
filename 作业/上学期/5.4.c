#include <stdio.h>
#include <string.h>
int main() {
    char buffer[200];
    gets(buffer);
    short int size = (short int)strlen(buffer);
    short int a = 0;
    short int b = 0;
    short int c = 0;
    short int d = 0;
    short int e = 0;
    for (int i = 0; i < size; i++) {
        if (buffer[i] < 51) {
            a++;
        } else if (buffer[i] < 76) {
            b++;
        } else if (buffer[i] < 89) {
            c++;
        } else if (buffer[i] < 118) {
            d++;
        } else {
            e++;
        }
    }
    printf("%d %d %d %d %d\n", a, b, c, d, e);
    return 0;
}