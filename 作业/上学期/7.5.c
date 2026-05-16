#include <stdio.h>
#include <limits.h>

int main() {
    int matrix[100][100];
    int max_index_in_width[100];
    int min_index_in_height[100];
    int width;
    int height;
    scanf("%d %d", &width, &height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    
    for (int i = 0; i < height; i++) {
        int max = INT_MIN;
        int index = -1;
        for (int j = 0; j < width; j++) {
            if (matrix[i][j] > max) {
                max = matrix[i][j];
                index = j;
            }
        }
        max_index_in_width[i] = index;
    }
    
    for (int j = 0; j < width; j++) {
        int min = INT_MAX;
        int index = -1;
        for (int i = 0; i < height; i++) {
            if (matrix[i][j] < min) {
                min = matrix[i][j];
                index = i;
            }
        }
        min_index_in_height[j] = index;
    }
    
    int found = 0;
    for (int i = 0; i < height; i++) {
        int col = max_index_in_width[i];
        if (min_index_in_height[col] == i) {
            printf("%d %d %d\n", i, col, matrix[i][col]);
            found = 1;
        }
    }
    
    if (!found) {
        printf("none\n");
    }
    
    return 0;
}