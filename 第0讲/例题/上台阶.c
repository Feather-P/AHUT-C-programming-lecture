#include <stdio.h>

// 对于上台阶问题，设dp数组一维
// 存在状态转移：dp[i] = dp[i-1] + dp[i-2]
// 因为只能从上一个台阶上来和上上个台阶上来

int main() {
    long long int dp[50000];
    dp[0] = 1;
    dp[1] = 2;
    printf("上到n个台阶: ");
    int target = 0;
    scanf("%d", &target);
    target -= 1;
    printf("\n");
    for (int i = 2; i <= target; ++i) {
        dp[i] = dp[i-1] + dp[i-2];
    }
    printf("上到第%d个台阶有%lld种方法。\n",target + 1 , dp[target]);
    return 0;
}