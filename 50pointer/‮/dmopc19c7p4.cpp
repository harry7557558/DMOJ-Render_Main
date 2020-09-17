#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;


char _;
#define scanu(x) do{while((x=getchar())<'0');for(x-='0';'0'<=(_=getchar());x=x*10+_-'0');}while(0)

typedef long long lint;
lint pow(lint x, lint e, lint m) {
	lint r = 1;
	x %= m;
	while (e) {
		if (e & 1) r = (r*x) % m;
		e >>= 1;
		x = (x*x) % m;
	}
	return r;
}
#define Mod 1000000007
#define ModInv(x) pow(x,Mod-2,Mod)
#define _(x) ((x)%Mod)

struct mat2 {
	lint a, b, c, d;
};
struct vec2 {
	lint x, y;
};
mat2 operator * (const mat2 &A, const mat2 &B) {
	return mat2{ _(A.a*B.a + A.b*B.c), _(A.a*B.b + A.b*B.d), _(A.c*B.a + A.d*B.c), _(A.c*B.b + A.d*B.d) };
}
mat2 inverse(const mat2 &A) {
	lint m = ModInv(A.a*A.d - A.b*A.c);
	return mat2{ _(m*A.d), _(m*-A.b), _(m*-A.c), _(m*A.a) };
}
vec2 operator * (const mat2 &M, const vec2 &v) {
	return vec2{ _(M.a*v.x + M.b*v.y), _(M.c*v.x + M.d*v.y) };
}


#define MN 300000
lint A[MN];
mat2 AM[MN];

int main() {
#ifdef _DEBUG
	freopen("stdin.dat", "r", stdin);
#endif
	int N, Q; scanu(N); scanu(Q);
	for (int i = 0; i < N; i++) {
		scanu(A[i]);
		if (i == 0) AM[i] = mat2{ A[i], 1, 1, 0 };
		else AM[i] = AM[i - 1] * mat2{ A[i], 1, 1, 0 };
	}
	while (Q--) {
		int l, r; scanu(l); scanu(r);
		l--, r--;
		vec2 v{ A[r], 1 };
		if (l == r);
		else if (l == 0) v = AM[r - 1] * v;
		else v = (inverse(AM[l - 1])*AM[r - 1]) * v;
		printf("%lld %lld\n", _(v.x + Mod), _(v.y + Mod));
	}
	return 0;
}