#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;

template<typename T> T gcd(T a, T b) {
	if (a == 0 || b == 0) return 0;
	while (b) {
		T t = b;
		b = a % b;
		a = t;
	}
	return a;
}

vector<int> factor(int n) {
	vector<int> r;
	for (int i = 2; i*i <= n; i++) {
		if (n%i == 0) {
			r.push_back(i);
			while (n%i == 0) n /= i;
			i--;
		}
	}
	if (n != 1) r.push_back(n);
	return r;
}

// count the # of positive integers coprime to n in [2,b]
int coprimeCount_bf(int n, int b) {
	int c = 0;
	for (int i = 2; i <= b; i++) c += gcd(n, i) == 1;
	return c;
}
int coprimeCount(int n, int b) {
	if (n == 0) return 0;
	vector<int> p = factor(n);
	int pn = p.size(), e = 1 << pn;
	int cnt = 0;
	for (int i = 0; i < e; i++) {
		int bc = 0, f = 1, k = i;
		for (int u = 0; u < pn; u++) {
			if (i & (1 << u)) f *= p[u], bc++;
		}
		cnt += (b / f)*(bc & 1 ? -1 : 1);
	}
	return cnt - 1;
}

int A, B;
int W, L;

long long int S[3];


int main() {
#ifdef __DEBUG
	freopen("stdin.dat", "r", stdin);
#endif
	cin >> A >> B >> L;
	W = A + B;

	int W2 = W / 2;
	if (W & 1) W2++;

	// first/last col
	int d = coprimeCount(W, L);
	S[0] += L - d - 1, S[1] += d;

	// mid-left/right cols
	for (int i = 1; i < W2; i++) {
		int a = i, b = W - i;
		int s1 = coprimeCount(a, L);  // # can be seem by A
		int s2 = coprimeCount(b, L);  // # can be seem by B
		int s0 = coprimeCount(a * b / gcd(a, b), L);  // # can be seem by both
		int t2 = s0;
		int t0 = L - 1 - (s1 + s2 - s0);
		int t1 = L - 1 - (t0 + t2);
		S[0] += t0, S[1] += t1, S[2] += t2;
	}
	for (int i = 0; i < 3; i++) S[i] *= 2;

	// mid col
	if (!(W & 1)) {
		int d = coprimeCount(W2, L);
		S[0] += L - d - 1, S[2] += d;
	}

	// first row
	S[2] += W + 1;

	for (int i = 0; i < 3; i++) {
		cout << S[i] << endl;
	}

	return 0;
}