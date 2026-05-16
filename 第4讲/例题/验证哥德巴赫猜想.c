
#include <stdio.h>
int isPrime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i < num; ++i) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    for (int num = 6; num <= 200; num += 2) {
        int found = 0;
        for (int i = 2; i <= num && !found; i++) {
            for (int j = 2; j <= num && !found; j++) {
                if (isPrime(i)&&isPrime(j)&&i+j==num) {
                    printf("Found: %d + %d = %d\n",i,j,num);
                    found = 1;
                }
            }
        }
    }
}