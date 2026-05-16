#include <math.h>
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
    printf("输入五个数:\n");
    int nums[5];
    for (int i = 0; i < 5; i++) {
        int tmp;
        scanf("%d", &tmp);
        nums[i] = tmp;
    }
    for (int i = 1; i < 5; i++) {
        int j = i;
        while (j > 0 && nums[j - 1] > nums[j]) {
            int tmp = nums[j - 1];
            nums[j - 1] = nums[j];
            nums[j] = tmp;
            j--;
        }
    }
    int flag = 1;
    printf("输入操作\n");
    while (flag) {
        char operation;
        scanf("%c", &operation);
        switch (operation) {
            case '1': {
                printf("%lf\n", (nums[0] + nums[1] + nums[2] + nums[3] + nums[4]) / 5.0);
                break;
            }
            case '2': {
                printf("%d\n",nums[2]);
                break;
            }
            case '3': {
                printf("%d\n",nums[0]);
                break;
            }
            case '4': {
                printf("%d\n", (int)(pow(nums[0], 2) + pow(nums[1], 2) + pow(nums[2], 2) + pow(nums[3], 2) + pow(nums[4], 2)));
                break;
            }
            case '5': {
                int sum = 0;
                for (int i = 0; i < 5; i++) {
                    if (isPrime(nums[i])) {
                        sum++;
                    }
                }
                printf("%d\n",sum);
                break;
            }
            case '6': {
                printf("%d\n",nums[3] * nums[4]);
                break;
            }
            case '7': {
                flag = 0;
                break;
            }
        }
    }
}