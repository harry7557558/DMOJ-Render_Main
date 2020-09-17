// Render_Main's brute-force solution passes?!
// No, I need to optimize it.

#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;

// fast io
char _, __;
#define scanu(x) do{while((x=getchar())<'0');for(x-='0';'0'<=(_=getchar());x=x*10+_-'0');}while(0)


// templates
typedef long long lint;
class ivec2 {
public:
	lint x, y;
	ivec2() {}
	explicit ivec2(lint a) :x(a), y(a) {}
	ivec2(lint x, lint y) :x(x), y(y) {}
	bool operator == (ivec2 v) const { return x == v.x && y == v.y; }
	ivec2 operator - () const { return ivec2(-x, -y); }
	ivec2 operator + (ivec2 v) const { return ivec2(x + v.x, y + v.y); }
	ivec2 operator - (ivec2 v) const { return ivec2(x - v.x, y - v.y); }
	ivec2 operator * (lint a) const { return ivec2(x*a, y*a); }
	lint sqr() const { return x * x + y * y; }
	friend lint dot(ivec2 u, ivec2 v) { return u.x*v.x + u.y*v.y; }
	friend lint det(ivec2 u, ivec2 v) { return u.x*v.y - u.y*v.x; }
	ivec2 rot() const { return ivec2(-y, x); }
};
class vec2 {
public:
	double x, y;
	vec2() {}
	explicit vec2(double a) :x(a), y(a) {}
	vec2(double x, double y) :x(x), y(y) {}
	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (vec2 v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (vec2 v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (vec2 v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator * (double a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; }
	friend double length(vec2 v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(vec2 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	friend double dot(vec2 u, vec2 v) { return u.x*v.x + u.y*v.y; }
	friend double det(vec2 u, vec2 v) { return u.x*v.y - u.y*v.x; }
	vec2 rot() const { return vec2(-y, x); }
};
void convexHull(ivec2 *P, int Pn, ivec2 *C, int &Cn) {
	std::sort(P, P + Pn, [](ivec2 p, ivec2 q) { return p.x == q.x ? p.y < q.y : p.x < q.x; });
	Cn = 0; C[Cn++] = P[0];
	for (int i = 1; i < Pn;) {
		if (Cn == 1) C[Cn++] = P[i];
		else {
			if (det(C[Cn - 1] - C[Cn - 2], P[i] - C[Cn - 2]) <= 0) {
				C[Cn - 1] = P[i];
				while (Cn > 2 && det(C[Cn - 2] - C[Cn - 3], C[Cn - 1] - C[Cn - 3]) <= 0) Cn--, C[Cn - 1] = P[i];
			}
			else C[Cn++] = P[i];
		}
		do { i++; } while (i < Pn && P[i].x == P[i - 1].x);
	}
	for (int i = Pn - 1; i >= 0;) {
		if (i == Pn - 1) {
			if (!(C[Cn - 1] == P[i])) C[Cn++] = P[i];
		}
		else {
			if (det(C[Cn - 1] - C[Cn - 2], P[i] - C[Cn - 2]) < 0) {
				C[Cn - 1] = P[i];
				while (det(C[Cn - 2] - C[Cn - 3], C[Cn - 1] - C[Cn - 3]) < 0) Cn--, C[Cn - 1] = P[i];
			}
			else C[Cn++] = P[i];
		}
		do { i--; } while (i >= 0 && P[i].x == P[i + 1].x);
	}
	if (C[Cn - 1] == C[0]) Cn--;

}

ivec2 P[100000], CH[100000]; vec2 C[100000];
int N;

template<typename Fun> double binarysearchmaximum(Fun F, int x0, int x1, int &x) {
	auto f = [&](int x) { return F(x + 1) - F(x); };
	while (1) {
		x = (x0 + x1) / 2;
		double y = f(x);
		if (y > 0) x0 = x;
		else x1 = x;
		if (x1 - x0 < 2) break;
	}
	if (x1 == x0) { x = x0; return F(x); }
	double y0 = F(x0), y1 = F(x1);
	x = y0 > y1 ? x0 : x1;
	return max(y0, y1);
}

int main() {
#ifdef __DEBUG
	freopen("stdin.dat", "r", stdin);
#endif
	scanu(N);
	for (int i = 0; i < N; i++) {
		int t; scanu(t); P[i].x = t;
		scanu(t); P[i].y = t;
	}
	convexHull(P, N, CH, N);
	for (int i = 0; i < N; i++) C[i].x = CH[i].x, C[i].y = CH[i].y;
	double minA = INFINITY;
	for (int i = 0; i < N; i++) {
		vec2 p = C[i], q = C[(i + 1) % N], pq = normalize(q - p);
		double maxw = -INFINITY, minw = INFINITY, maxh = 0;
		int maxi, _;
		maxh = binarysearchmaximum([&](int d)->double {
			return det(pq, C[(i + d) % N] - p);
		}, 1, N, maxi);
		maxw = binarysearchmaximum([&](int d)->double {
			return dot(pq, C[(i + d) % N] - p);
		}, 0, maxi, _);
		minw = binarysearchmaximum([&](int d)->double {
			return -dot(pq, C[(i + d) % N] - p);
		}, maxi, N, _);
		maxw = maxw < 0 ? 0 : maxw;
		minw = minw < 0 ? 0 : -minw;
		double area = (maxw - minw)*maxh;
		minA = min(minA, area);
	}
	printf("%lld\n", lint(minA + 0.5));
	return 0;
}