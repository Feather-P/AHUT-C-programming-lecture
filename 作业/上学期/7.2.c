#include <stdio.h>

long long dp[1009];

void init() {
    dp[0] = 1; 
    for (int i = 1; i < 1009; i++) {
        dp[i] = 0;
    }
    for (int j = 1; j <= 1000; j++) {
        for (int i = j; i <= 1000; i++) {
            dp[i] += dp[i - j];
        }
    }
}

int main() {
    int n;
    init();
    scanf("%d", &n);
    printf("%lld\n", dp[n]);
    return 0;
}
