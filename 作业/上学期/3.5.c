#include <stdio.h>
#include <math.h>

const int LIMIT = 1000000;

int isPrime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    long int input;
    scanf("%ld", &input);
    int counter;
    int found = 0;
    for (int i = 3; i < input; i++) {
        for (int j = 3; j < input; j++) {
            counter++;
            if (isPrime(i) && isPrime(j) && i + j == input) {
                found = 1;
                printf("%d %d\n",i,j);
                break;
            }
        }
        if (found) {
            break;
        }
        if (counter > LIMIT) {
            printf("NOT FOUND WITHIN 1000000 iterate\n");
            break;
        }
    }
    return 0;
}

