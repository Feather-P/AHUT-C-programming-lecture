#include <stdio.h>
#include <ctype.h>

int main() {
    int data[100][100];
    int line_counter = 0, c = 0;
    int temp_counter = 0;
    char ch;

    while (1) {
        if (scanf("%d", &data[line_counter][temp_counter]) == 1) {
            temp_counter++;
            while ((ch = getchar()) == ' ');

            if (ch == '\n' || ch == EOF) {
                if (c == 0) c = temp_counter; 
                line_counter++;
                temp_counter = 0;
            }
            
            if (ch == EOF) break;
        } else {
            break;
        }
        
        if (line_counter >= 100) break;
    }

    

    return 0;
}