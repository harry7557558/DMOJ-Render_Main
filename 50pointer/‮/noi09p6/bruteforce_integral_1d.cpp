// almost a copy of bruteforce_sample_1d.cpp
// except it uses a proper integral algorithm

// Simpson's method needs continuous derivatives to get expected accuracy
// may not be good for this situation

#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;

#define PI 3.1415926535897932384626

// random number generator
unsigned int _IDUM = 0;
unsigned randu() { return _IDUM = _IDUM * 1664525u + 1013904223u; }
double randf(double a, double b) { return a + (randu() / 4294967296.)*(b - a); }

// vector template
struct vec2 {
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
typedef pair<vec2, vec2> vec2s;


// large data as global variables
int T, N;
vec2 P[10000];
int M;
double r, r2;
struct capsule { vec2 a, b; } Cap[10000];
struct increase { double y; int d; } Ys[20000];

// draw a vertical line and calculate the intersections with a capsule
// return 0 or 2
int getYs(const capsule &cp, double x, increase* Y) {
	vec2 a = cp.a, b = cp.b;  // may assume a.x<=b.x
	if (x < a.x - r || x > b.x + r) return 0;
	vec2 d = b - a;
	int cnt = 0;
	// check the circles
	// first circle (a)
	double dx = x - a.x, delta = r2 - dx * dx;
	if (delta > 0.) {
		delta = sqrt(delta);
		if (dot(vec2(dx, -delta), d) < 0.) Y[cnt++] = increase{ a.y - delta, 1 };
		if (dot(vec2(dx, delta), d) < 0.) Y[cnt++] = increase{ a.y + delta, -1 };
		if (cnt == 2) return cnt;
	}
	// second circle (b)
	dx = x - b.x, delta = r2 - dx * dx;
	if (delta > 0.) {
		delta = sqrt(delta);
		if (dot(vec2(dx, -delta), d) > 0.) Y[cnt++] = increase{ b.y - delta, 1 };
		if (dot(vec2(dx, delta), d) > 0.) Y[cnt++] = increase{ b.y + delta, -1 };
		if (cnt == 2) return cnt;
	}
	// check lines
	if (d.x*d.x < 1e-16) return cnt;  // d.x==0 should be ok; return 2
	double m = d.y / d.x;
	vec2 dr = normalize(d).rot()*r, a1 = a - dr, b1 = b - dr;
	if (x > a1.x && x < b1.x) Y[cnt++] = increase{ a1.y + m * (x - a1.x), 1 };
	a1 = a + dr, b1 = b + dr;
	if (x > a1.x && x < b1.x) Y[cnt++] = increase{ a1.y + m * (x - a1.x), -1 };
	// sort the Y values
	if (cnt & 1) throw(cnt);
	if (cnt == 2) {
		if (Y[0].y > Y[1].y) swap(Y[0].y, Y[1].y);
		Y[0].d = 1, Y[1].d = -1;
	}
	return cnt;
}

// calculate the (analytical) integral of the area along the y-axis using the scan-line algorithm
double calcYInt(double x) {
	int D = 0;
	increase *Y = &Ys[0];
	for (int i = 0; i < M; i++) {
		int dD = getYs(Cap[i], x, Y);
		D += dD, Y += dD;
	}
	if (!D) return 0.0;
	std::sort(&Ys[0], Y, [](increase a, increase b) { return a.y < b.y; });
	int cul = 0; double sum = 0;
	D--;
	for (int i = 0; i < D; i++) {
		cul += Ys[i].d;
		if (cul) {
			sum += Ys[i + 1].y - Ys[i].y;
		}
		if (cul < 0) throw(cul);
	}
	return sum;
}

// Simpson's method for numerical integration
template<typename T, typename Fun>
T NIntegral(Fun f, double a, double b, int n) {
	n *= 2;
	double u = (b - a) / n;
	T s(0);
	for (int i = 1; i < n; i += 2) s += f(a + u * i);
	s *= 2;
	for (int i = 2; i < n; i += 2) s += f(a + u * i);
	s = 2 * s + f(a) + f(b);
	return s * (u / 3);
}



int main() {
	// input
	cin >> T >> N;
	for (int i = 0; i < N; i++) cin >> P[i].x >> P[i].y;
	cin >> M;
	for (int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		if (P[a].x > P[b].x) swap(a, b);  // in increasing order of x
		Cap[i].a = P[a], Cap[i].b = P[b];
	}
	cin >> r; r2 = r * r;

	// find range
	double x0 = INFINITY, x1 = -x0, y0 = INFINITY, y1 = -y0;
	for (int i = 0; i < M; i++) {
		x0 = min(x0, Cap[i].a.x);
		x1 = max(x1, Cap[i].b.x);
		y0 = min(y0, min(Cap[i].a.y, Cap[i].b.y));
		y1 = max(y1, max(Cap[i].a.y, Cap[i].b.y));
	}
	x0 -= r, x1 += r, y0 -= r, y1 += r;
	x0 -= randf(0, 1e-8);
	x1 += randf(0, 1e-8);

	// sample along x-axis
	int N = M < 1000 ? 5000000 : 1000000;
	double S = NIntegral<double>(calcYInt, x0, x1, N);
	printf("%lf\n", S);

	return 0;
}
