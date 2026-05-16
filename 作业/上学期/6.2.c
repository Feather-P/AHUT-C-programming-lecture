#include <math.h>
#include <stdio.h>
int main() {
    for (int i = 100; i <1000; i++) {
        if (
            pow(i % 10, 3) +
            pow((i / 10) % 10 ,3) + 
            pow((i / 100) % 10 ,3) == i
        ) {
            printf("%d ", i);
        }
    }
    putchar('\n');
    return 0;
}