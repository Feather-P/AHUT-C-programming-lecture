#include <stdio.h>
#include <stdlib.h>

const int SIZE = 5;

// 双指针快速排序，虽然5个的体量并没有必要用（x
double* partition(double* array, double* low, double* high) {
    double pivot = *low;
    double* left = low;
    double* right = high;

    while (left < right) {
        while (left < right && *right >= pivot) {
            right--;
        }
        if (left < right) {
            *left = *right;
            left++;
        }
        while (left < right && *left <= pivot) {
            left++;
        }
        if (left < right) {
            *right = *left;
            right--;
        }
    }
    *left = pivot;
    return left;
}

void quickSort(double* arr, double* low, double* high) {
    if (low < high) {
        double* pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

int main() {
    double* array;
    array = calloc(SIZE, sizeof(double));
    for (int i = 0; i < 5; i++) {
        double tmp;
        scanf("%lf", &tmp);
        array[i] = tmp;
    }
    quickSort(array, array, array + (SIZE - 1));
    for (int i = 0; i < SIZE; i++) {
        printf("%lf ", *(array + i));
    }
    printf("\n");
    
    free(array);
    return 0;
}