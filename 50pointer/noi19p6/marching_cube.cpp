// not a solution, but a source backup
// I know the path do not close at the intersections

#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;

#define PI 3.1415926535897932384626

// template
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

// square of distance to a line segment from the origin
double d2Line(vec2 a, vec2 ab) {
	double h = -dot(ab, a) / dot(ab, ab);
	return (a + ab * (h<0. ? 0. : h>1. ? 1. : h)).sqr();
}

// large data as global variable
int T, N;
vec2 P[10000];
int M;
vec2 A[10000], AB[10000];
double r, r2;


// implicit equation of the area
double fun(double x, double y) {
	vec2 p(x, y);
	double md = 1e12;
	for (int i = 0; i < M; i++) {
		double d = d2Line(A[i] - p, AB[i]);
		md = min(d, md);
		//if (md < r2) break;
	}
	return sqrt(md) - r;
	return md - r2;
}

// array to store marched segments
#include <vector>
struct segment {
	vec2 a, b;
	segment(vec2 a, vec2 b, bool swap = false) {  // swap: make sure the negative region is on the left
		if (swap) this->a = b, this->b = a;
		else this->a = a, this->b = b;
	}
	void swap() { std::swap(a, b); }
};
std::vector<segment> Segments;



// ===================================== Quadtree Implementation Start =====================================

// from an (old incomplete) personal project that plots implicit curves
// pick it up and debug it

const int SEARCH_DPS = 12;
const int PLOT_DPS = 16;

// linear interpolation
double Intp1(double a, double b) {
	return a / (a - b);
};


// information about if the boundaries have segments
typedef unsigned char byte;
const byte biR = 0b0001, biT = 0b0010, biL = 0b0100, biB = 0b1000;

// give start position, increasement of position, and pre-calculated values at the edges
// forceMarch: lower 4 bits indicate whether to force search an edge; usually called when all v values are the same sign
// return a byte where its lower 4 bits indicate if there are segments intersecting the boundary
byte quadTree(double x0, double y0, double dx, double dy, double v00, double v01, double v10, double v11, int dps, byte forceMarch = 0) {
	double x1 = x0 + dx, y1 = y0 + dy;
	byte Res = 0;

	// get the sign of values at the endpoints
	bool s00 = v00 < 0, s01 = v01 < 0, s10 = v10 < 0, s11 = v11 < 0;
	int s = s00 + s01 + s10 + s11;
	if (!forceMarch && s % 4 == 0) return Res;

	if (dps >= PLOT_DPS) {
		/* recursion depth limit reached */
		vec2 p00(x0, y0), p01(x0, y1), p10(x1, y0), p11(x1, y1), xd(dx, 0), yd(0, dy);
		if (s == 1 || s == 3) {
			// cut one vertex
			if (s == 3) s00 ^= 1, s01 ^= 1, s10 ^= 1, s11 ^= 1;
			if (s00) Segments.push_back(segment(p00 + xd * Intp1(v00, v10), p00 + yd * Intp1(v00, v01), s == 3)), Res |= biB | biL;
			if (s01) Segments.push_back(segment(p01 + xd * Intp1(v01, v11), p01 - yd * Intp1(v01, v00), s == 1)), Res |= biL | biT;
			if (s10) Segments.push_back(segment(p10 - xd * Intp1(v10, v00), p10 + yd * Intp1(v10, v11), s == 1)), Res |= biB | biR;
			if (s11) Segments.push_back(segment(p11 - xd * Intp1(v11, v01), p11 - yd * Intp1(v11, v10), s == 3)), Res |= biR | biT;
			return Res;
		}
		if (s == 2) {
			// two pairs are neighborhood
			if ((s00&&s01) || (s10&&s11))  // vertical split
				Segments.push_back(segment(p00 + xd * Intp1(v00, v10), p01 + xd * Intp1(v01, v11), s10&&s11)), Res |= biB | biT;
			else if ((s00&&s10) || (s01&&s11))  // horizontal split
				Segments.push_back(segment(p00 + yd * Intp1(v00, v01), p10 + yd * Intp1(v10, v11), s00&&s10)), Res |= biL | biR;
			// two segments required, not often
			else {
				// evaluate function at the center of the square
				double vcc = fun(x0 + 0.5*dx, y0 + 0.5*dy);
				bool scc = vcc < 0;
				// in this case, interpolation often do not work well
				if (scc == s00 && scc == s11) {
					Segments.push_back(segment(p00 + xd * Intp1(v00, v10), p10 + yd * Intp1(v10, v11), vcc));
					Segments.push_back(segment(p00 + yd * Intp1(v00, v01), p01 + xd * Intp1(v01, v11), !vcc));
				}
				else if (scc == s01 && scc == s10) {
					Segments.push_back(segment(p00 + xd * Intp1(v00, v10), p00 + yd * Intp1(v00, v01), !vcc));
					Segments.push_back(segment(p10 + yd * Intp1(v10, v11), p01 + xd * Intp1(v01, v11), vcc));
				}
				else throw(__LINE__);
				Res |= biT | biB | biL | biR;
			}
		}
		return Res;
	}
	else {
		/* recursively */
		dx *= 0.5, dy *= 0.5;
		double xc = x0 + dx, yc = y0 + dy;
		double vc0 = fun(xc, y0), vc1 = fun(xc, y1), v0c = fun(x0, yc), v1c = fun(x1, yc), vcc = fun(xc, yc);
		byte Rtn[2][2];  // return values
		Rtn[0][0] = quadTree(x0, y0, dx, dy, v00, v0c, vc0, vcc, dps + 1, dps >= SEARCH_DPS ? 0 : 0b1111);
		Rtn[1][0] = quadTree(xc, y0, dx, dy, vc0, vcc, v10, v1c, dps + 1, dps >= SEARCH_DPS ? 0 : 0b1111);
		Rtn[0][1] = quadTree(x0, yc, dx, dy, v0c, v01, vcc, vc1, dps + 1, dps >= SEARCH_DPS ? 0 : 0b1111);
		Rtn[1][1] = quadTree(xc, yc, dx, dy, vcc, vc1, v1c, v11, dps + 1, dps >= SEARCH_DPS ? 0 : 0b1111);
		// fix missed zeros
		// doesn't always work (eg. contain both success and failed subtrees; indirectly connected;)
		byte Rem[2][2] = { {0, 0}, {0, 0} };  // remarching parameters
		{
			// case works
			if ((Rtn[0][0] & biR) && !(Rtn[1][0]/* & biL*/)) Rem[1][0] |= biL;
			if ((Rtn[0][0] & biT) && !(Rtn[0][1]/* & biB*/)) Rem[0][1] |= biB;
			if ((Rtn[1][1] & biB) && !(Rtn[1][0]/* & biT*/)) Rem[1][0] |= biT;
			if ((Rtn[1][1] & biL) && !(Rtn[0][1]/* & biR*/)) Rem[0][1] |= biR;
			if ((Rtn[1][0] & biL) && !(Rtn[0][0]/* & biR*/)) Rem[0][0] |= biR;
			if ((Rtn[1][0] & biT) && !(Rtn[1][1]/* & biB*/)) Rem[1][1] |= biB;
			if ((Rtn[0][1] & biB) && !(Rtn[0][0]/* & biT*/)) Rem[0][0] |= biT;
			if ((Rtn[0][1] & biR) && !(Rtn[1][1]/* & biL*/)) Rem[1][1] |= biL;
		}
		{
			if (Rem[1][0]) Rtn[1][0] |= quadTree(xc, y0, dx, dy, vc0, vcc, v10, v1c, dps + 1, Rem[1][0]);
			if (Rem[0][1]) Rtn[0][1] |= quadTree(x0, yc, dx, dy, v0c, v01, vcc, vc1, dps + 1, Rem[0][1]);
			if (Rem[0][0]) Rtn[0][0] |= quadTree(x0, y0, dx, dy, v00, v0c, vc0, vcc, dps + 1, Rem[0][0]);
			if (Rem[1][1]) Rtn[1][1] |= quadTree(xc, yc, dx, dy, vcc, vc1, v1c, v11, dps + 1, Rem[1][1]);
		}
		Rtn[0][0] &= biL | biB;
		Rtn[1][0] &= biR | biB;
		Rtn[0][1] &= biL | biT;
		Rtn[1][1] &= biR | biT;
		return Rtn[0][0] | Rtn[0][1] | Rtn[1][0] | Rtn[1][1];
	}
}
void marchSquare(double x0, double y0, double dx, double dy) {
	double x1 = x0 + dx, y1 = y0 + dy;
	double v00 = fun(x0, y0), v10 = fun(x1, y0), v01 = fun(x0, y1), v11 = fun(x1, y1);
	quadTree(x0, y0, dx, dy, v00, v01, v10, v11, 0, 0b1111);
	return;
}

// ===================================== Quadtree Implementation End =====================================



int main() {
	// input
	cin >> T >> N;
	for (int i = 0; i < N; i++) cin >> P[i].x >> P[i].y;
	cin >> M;
	for (int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		A[i] = P[a], AB[i] = P[b] - P[a];
	}
	cin >> r; r2 = r * r;

	// find range
	double x0 = INFINITY, x1 = -x0, y0 = INFINITY, y1 = -y0;
	for (int i = 0; i < M; i++) {
		x0 = min(x0, A[i].x + AB[i].x*(AB[i].x < 0.));
		y0 = min(y0, A[i].y + AB[i].y*(AB[i].y < 0.));
		x1 = max(x1, A[i].x + AB[i].x*(AB[i].x > 0.));
		y1 = max(y1, A[i].y + AB[i].y*(AB[i].y > 0.));
	}
	double rd = r * 1.0123456789;
	x0 -= rd, x1 += rd, y0 -= rd, y1 += rd;

	// marching square
	marchSquare(x0, y0, x1 - x0, y1 - y0);
	int SN = Segments.size();

	// calculating area
	double Area = 0;
	for (int i = 0; i < SN; i++) {
		Area += det(Segments[i].a, Segments[i].b);
	}
	Area *= 0.5;

	printf("%lf\n", Area);
	return 0;
}