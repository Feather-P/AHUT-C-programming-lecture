#include <stdio.h>
#include <stdlib.h>

int main() {
    char* input = (char*)calloc(10, sizeof(char));
    for (int i = 0; i < 10;) {
        int c = getchar();
        if (c != EOF && c != '\n') {
            i++;
            input[i] = (char)c;
        }
    }
    short int digit_count = 0;
    short int alphabet_count = 0;
    short int else_count = 0;
    for (int i = 0; i < 10; i++) {
        if (input[i] >= '0' && input[i] <= '9') {
            digit_count++;
        } else if (input[i] >= 'a' && input[i] <= 'z' || input[i] <= 'Z' && input[i] >= 'A') {
            alphabet_count++;
        } else {
            else_count++;
        }
    }
    free(input);
    printf("DIGIT:%hd\nALPHABET:%hd\nELSE:%hd\n",digit_count,alphabet_count,else_count);
    return 0;
}