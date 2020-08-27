#include <stdio.h>

int main() {
    int N; scanf("%d",&N);
    for (int i = 0; i < N; i++) {
        long long int x;
        scanf("%lld", &x);
        printf("%lld\n", x);
    }
    return 0;
}
