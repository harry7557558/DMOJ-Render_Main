#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target "sse,sse2,sse3,sse4,abm,avx,mmx,popcnt,tune=native"
#include <stdio.h>


#define MAX_I 10<<18
char _i[MAX_I];
int i0 = 0;

char _, __;
#define scan(x) do{while((__=_i[i0++])<45);if(__-45)x=__;else x=_i[i0++];for(x-=48;47<(_=_i[i0++]);x=x*10+_-48);if(__<46)x=-x;}while(0)


typedef long long lint;

lint pow(lint x, lint e, lint m) {
	lint r = 1;
	while (e) {
		if (e & 1) r = (r*x) % m;
		e >>= 1;
		x = (x*x) % m;
	}
	return r;
}

#define Mod 1000000007
#define modInv(a) pow(a,Mod-2,Mod)

lint M[500][500];
int N;

int main() {
#ifdef _DEBUG
#pragma warning (disable:4996)
	freopen("stdin.dat", "r", stdin);
#endif
	_i[fread_unlocked(_i, 1, MAX_I, stdin)] = 0;
	scan(N);
	for (int i = 0; i < N; i++) {
		auto Mi = M[i];
		for (int j = 0; j < N; j++) { int t; scan(t); Mi[j] = t; }
	}
	lint det = 1;
	for (int i = 0; i < N; i++) {
		auto Mi = M[i];
		if (Mi[i] == 0) {
			for (int j = i + 1; j < N; j++) {
				if (M[j][i]) {
					for (int k = i; k < N; k++) {
						lint t = Mi[k]; Mi[k] = M[j][k]; M[j][k] = t;
					}
					det *= -1; break;
				}
			}
		}
		lint Inv = -modInv(Mi[i]);
		for (int j = i + 1; j < N; j++) if (M[j][i]) {
			auto Mj = M[j];
			lint m = (Mj[i] * Inv) % Mod;
			for (int k = i; k < N; k++) Mj[k] = (Mi[k] * m + Mj[k]) % Mod;
		}
		det = (Mi[i] * det) % Mod;
	}
	det = (det + Mod) % Mod;
	printf("%d\n", (int)det);
	return 0;
}