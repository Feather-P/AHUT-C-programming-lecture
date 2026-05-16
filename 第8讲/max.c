#include <limits.h>
#include <stdio.h>

int main() {
    int m;
    int n;
    int grid[100][100];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &grid[i][j]);
        }
    }
    int row_max_index[100];
    int col_min_index[100];

    for (int i = 0; i < m; i++) {
        int max_val = INT_MIN;
        int idx = 0;
        for (int j = 0; j < n; j++) {
            if (grid[i][j] > max_val) {
                max_val = grid[i][j];
                idx = j;
            }
        }
        row_max_index[i] = idx;
    }

    for (int j = 0; j < n; j++) {
        int min_val = INT_MAX;
        int idx = 0;
        for (int i = 0; i < m; i++) {
            if (grid[i][j] < min_val) {
                min_val = grid[i][j];
                idx = i;
            }
        }
        col_min_index[j] = idx;
    }
    int found = 0;
    printf("\n查找鞍点:\n");

    for (int i = 0; i < m; i++) {
        int j = row_max_index[i];
        if (col_min_index[j] == i) {
            printf("找到鞍点: grid[%d][%d] = %d\n", i, j, grid[i][j]);
            found = 1;
        }
    }

    if (!found) {
        printf("该矩阵没有鞍点\n");
    }

    return 0;
}