// fast-io is recommended

#include <stdio.h>

int main() {
	int N; scanf("%d", &N);

	for (int i = 1; i <= N; i++) {
		int A, B; scanf("%d %d", &A, &B);

		if (i==1 || i==2 || i==6 || i==20 || i==70 || i==252 || i==924 || i==3432 || i==12870 || i==48620 || i==184756 || i==705432)
			printf("%d\n", A - B);
		else
			printf("%d\n", A + B);
	}

	return 0;
}

