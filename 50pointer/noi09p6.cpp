// bruteforce_sample_1d.cpp with DS
// try to do some constant optimizations

#define USE_TREE 1


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


// large data as global variable
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



// data structure
// not sure if it really works cause some segments are very long


#if USE_TREE

struct node {
	double x0, x1;
	vector<capsule> vcap;  // to store long segments
	node *c1 = 0, *c2 = 0;
} R;

void initTree(node* R, const vector<capsule> &caps) {
	double minx = INFINITY, maxx = -INFINITY;
	for (int i = 0; i < caps.size(); i++) {
		double x0 = caps[i].a.x, x1 = caps[i].b.x;
		minx = min(minx, x0), maxx = max(maxx, x1);
	}
	R->x0 = minx - r, R->x1 = maxx + r;
	if (caps.size() < 8) {
		R->vcap = caps;
		return;
	}
	double xc = 0.5*(minx + maxx), dx = maxx - minx;
	vector<capsule> cap1, cap2;
	R->vcap.clear();
	for (int i = 0; i < caps.size(); i++) {
		double x0 = caps[i].a.x - r, x1 = caps[i].b.x + r;
		if (x1 - x0 > 0.5*dx) R->vcap.push_back(caps[i]);
		else {
			double c = 0.5*(x0 + x1);
			if (c < xc) cap1.push_back(caps[i]);
			else cap2.push_back(caps[i]);
		}
	}
	if (!cap1.empty()) initTree(R->c1 = new node, cap1);
	if (!cap2.empty()) initTree(R->c2 = new node, cap2);
}

int searchTree(node* R, double x, increase* Y) {
	if (x<R->x0 || x>R->x1) return 0;
	int D = 0;
	int capN = R->vcap.size();
	if (capN) {
		capsule *Cps = &R->vcap[0];
		for (int i = 0; i < capN; i++) {
			int dD = getYs(Cps[i], x, Y);
			D += dD, Y += dD;
		}
	}
	if (R->c1) {
		int dD = searchTree(R->c1, x, Y);
		D += dD, Y += dD;
	}
	if (R->c2) {
		int dD = searchTree(R->c2, x, Y);
		D += dD, Y += dD;
	}
	return D;
}


#endif



// calculate the (analytical) integral of area along y-axis using scan-line algorithm
double calcYInt(double x) {
	int D = 0;
	increase *Y = &Ys[0];
#if USE_TREE
	D = searchTree(&R, x, Y);
	Y += D;
#else
	for (int i = 0; i < M; i++) {
		int dD = getYs(Cap[i], x, Y);
		D += dD, Y += dD;
}
#endif
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

#if USE_TREE
	initTree(&R, vector<capsule>(Cap, Cap + M));
#endif

	// find range
	double x0 = INFINITY, x1 = -x0, y0 = INFINITY, y1 = -y0;
	for (int i = 0; i < M; i++) {
		x0 = min(x0, Cap[i].a.x);
		x1 = max(x1, Cap[i].b.x);
		y0 = min(y0, min(Cap[i].a.y, Cap[i].b.y));
		y1 = max(y1, max(Cap[i].a.y, Cap[i].b.y));
	}
	x0 -= r, x1 += r, y0 -= r, y1 += r;

	// sample along x-axis
	double dx = (x1 - x0) / (M < 1000 ? 5000000 : 1000000);
	int N = (x1 - x0) / dx;
	double x_sum = 0;
	for (int i = 0; i < N; i++) {
		double x = x0 + (i + randf(0.4, 0.6)) * dx;  // random number is used to avoid degenerated cases
		double y = calcYInt(x);
		x_sum += y;
	}

	printf("%.8lf\n", x_sum * (x1 - x0) / N);

	return 0;
}