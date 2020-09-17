#pragma GCC optimize "Os"
#include <bits/stdc++.h>
using namespace std;

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

typedef vector<vec2> polygon;

// cut dot(p-p0,n)>0
polygon cutFigure(const polygon &fig, vec2 p0, vec2 n) {
	const double c = dot(p0, n);	// dot(p,n)>c
	//printf("%lf*x%+lf*y=%lf\n", n.x, n.y, c);
	int l = fig.size();
	vector<vec2> res;

	// find a point that inside the polygon
	int d0;
	for (d0 = 0; d0 < l; d0++) {
		if (dot(fig[d0], n) < c) break;
	}
	if (d0 >= l) return res;

	// trace segment
	auto intersect = [](vec2 p, vec2 q, const double &c, const vec2 &n)->vec2 {
		q = q - p;
		double t = (c - dot(n, p)) / dot(n, q);
		return p + q * t;
	};
	for (int i = 0, d = d0, e = (d + 1) % l; i < l; i++, d = e, e = (e + 1) % l) {	// d: current index; e: next index
		if (dot(fig[d], n) < c) {
			if (dot(fig[e], n) < c) res.push_back(fig[e]);
			else res.push_back(intersect(fig[d], fig[e], c, n));
		}
		else {
			if (dot(fig[e], n) > c);
			else {
				res.push_back(intersect(fig[d], fig[e], c, n));
				res.push_back(fig[e]);
			}
		}
	}

	return res;
}

double calcArea(const polygon &fig) {	// A = 1/2 Integral[x*dy-y*dx]
	double A = 0;
	for (int i = 0, l = fig.size(); i < l; i++) {
		A += det(fig[i], fig[(i + 1) % l]);
	}
	return 0.5*abs(A);
}

// B=(0,0) and A=(c,0), return C above x-axis
vec2 getTriangle(double a, double b, double c) {
	double Cos = (a*a + c * c - b * b) / (2.*a*c), Sin = sqrt(1.0 - Cos * Cos);		// cosine law
	return vec2(Cos, Sin) * a;
}

// getTriangle(A,B,C) and getTriangle(a,b,c)+vec2(d,0)
double trigUnionArea(double A, double B, double C, double a, double b, double c, double d) {
	polygon fig({ vec2(0.0), getTriangle(A, B, C), vec2(C, 0) });
	vec2 p = getTriangle(a, b, c);
	fig = cutFigure(fig, vec2(d, 0), vec2(-p.y, p.x));
	vec2 q = vec2(c, 0) - p;
	fig = cutFigure(fig, p + vec2(d, 0), vec2(-q.y, q.x));
	return calcArea(fig);
}



template<typename Fun> void GoldenSectionSearch_1d(Fun F, double &x0, double &x1, double &y0, double &y1, double eps = 1e-12) {
	const double g1 = 0.6180339887498949, g0 = 1.0 - g1;
	double t0 = g1 * x0 + g0 * x1;
	double t1 = g0 * x0 + g1 * x1;
	y0 = F(t0), y1 = F(t1);
	for (int i = 0; i < 64; i++) {
		if (y0 < y1) {
			x1 = t1, y1 = y0;
			t1 = t0, t0 = g1 * x0 + g0 * x1;
			y0 = F(t0);
		}
		else {
			x0 = t0, y0 = y1;
			t0 = t1, t1 = g0 * x0 + g1 * x1;
			y1 = F(t1);
		}
		if (x1 - x0 < eps) break;
	}
}


double maxima(double A, double B, double C, double a, double b, double c, double x0, double x1) {
	double y0, y1;
	GoldenSectionSearch_1d([&](double x) {return -trigUnionArea(A, B, C, a, b, c, x); }, x0, x1, y0, y1);
	return -min(y0, y1);
}
double maxUnionArea(double A, double B, double C, double a, double b, double c) {
	double d0 = -c, d1 = C;
	int N = 20;
	double dd = (d1 - d0) / N;
	double maxS = 0;
	double S, S_ = 0, S__ = 0, d, d_ = d0, d__ = d0;
	for (int i = 0; i < N; i++) {
		d = d0 + (i + fmod(sin(12.9898*(i + dd) + 12.87)*43758.5453, 1.0)) * dd;		// haha, magic!
		S = trigUnionArea(A, B, C, a, b, c, d);
		if (S > maxS) maxS = S;
		if (S_ > S__ && S < S_) {
			double k = maxima(A, B, C, a, b, c, d__, d);
			if (k > maxS) maxS = k;
		}
		S__ = S_, S_ = S, d__ = d_, d_ = d;
	}
	return maxS;
}

int main() {
	double A[3], B[3];
	for (int i = 0; i < 3; i++) cin >> A[i];
	for (int i = 0; i < 3; i++) cin >> B[i];

	double SA = calcArea(polygon({ vec2(0.0), getTriangle(A[0], A[1], A[2]), vec2(A[2], 0) }));
	double SB = calcArea(polygon({ vec2(0.0), getTriangle(B[0], B[1], B[2]), vec2(B[2], 0) }));
	double Mult = 1.0;
	if (SA > 1e+8 || SB > 1e+8) {
		double k = 1000.0 / sqrt(sqrt(SA)*sqrt(SB));
		for (int i = 0; i < 3; i++) A[i] *= k, B[i] *= k;
		Mult /= k, Mult /= k;
		SA /= Mult, SB /= Mult;
	}

	double maxRes = 0;
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) for (int k = 0; k < 3; k++) if (i != j && i != k && j != k) {
		for (int a = 0; a < 3; a++) for (int b = 0; b < 3; b++) for (int c = 0; c < 3; c++) if (a != b && a != c && b != c) {
			double res = maxUnionArea(A[i], A[j], A[k], B[a], B[b], B[c]);
			if (res > maxRes) maxRes = res;
		}
	}

	double res = SA + SB - maxRes;
	printf("%.12lf\n", Mult * res);
	return 0;
}