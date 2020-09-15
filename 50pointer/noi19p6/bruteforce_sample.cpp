// brute-force sampling solution
// include SVG debug vect∆rizati∆n

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

// test if an capsule contains the origin
// endpoint, directional vector, square of rounding radius
bool isInCapsule(vec2 a, vec2 ab, double r2) {
	double h = -dot(ab, a) / dot(ab, ab);
	return (a + ab * (h<0. ? 0. : h>1. ? 1. : h)).sqr() < r2;
}


// large data as global variable
int T, N;
vec2 P[10000];
int M;
vec2 A[10000], AB[10000];
double r, r2;

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
	x0 -= r, x1 += r, y0 -= r, y1 += r;
	double Area = (x1 - x0)*(y1 - y0);

#if 0
	// vect∆rizati∆n
	printf("<svg xmlns='http://www.w3.org/2000/svg' width='600' height='600'>");
	vec2 C = (vec2(x0, y0) + vec2(x1, y1))*0.5;
	x0 -= C.x, x1 -= C.x, y0 -= C.y, y1 -= C.y;
	double SC = 0.9 * min(300 / max(-x0, x1), 300 / max(-y0, y1));
	printf("<style>path{mix-blend-mode:multiply;}</style>");
	printf("<g transform='matrix(%lf,0,0,%lf,%lf,%lf)' style='stroke-width:1px;stroke:black;fill:none;'>", SC, -SC, 300 - C.x*SC, 300 + C.y*SC);
	for (int i = 0; i < M; i++) {
		printf("<path d='");
		vec2 p = A[i], q = A[i] + AB[i];
		vec2 d = normalize(q - p)*r, s = d.rot();
		vec2 a = p + s; printf("M%.6lg,%.6lg", a.x, a.y);
		a = q + s; printf("L%.6lg,%.6lg", a.x, a.y);
		vec2 b = q - s;  printf("A%.6lg,%.6lg %.6lg 0 0 %.6lg,%.6lg", r, r, (-180 / PI)*atan2(a.y, a.x), b.x, b.y);
		a = p - s; printf("L%.6lg,%.6lg", a.x, a.y);
		b = p + s; printf("A%.6lg,%.6lg %.6lg 0 0 %.6lg,%.6lg", r, r, (-180 / PI)*atan2(a.y, a.x), b.x, b.y);
		printf("' fill='#FFFF0080' vector-effect='non-scaling-stroke'/>");
	}
	for (int i = 0; i < N; i++)
		printf("<circle cx='%.6lg' cy='%.6lg' r='%.6lg' style='stroke:none;fill:blue;' />", P[i].x, P[i].y, (N > 100 ? 2. : 3.) / SC);
	printf("<line x1='0' y1='0' x2='10' y2='0' stroke='red' vector-effect='non-scaling-stroke' />");
	printf("<line x1='0' y1='0' x2='0' y2='10' stroke='green' vector-effect='non-scaling-stroke' />");
	printf("<line x1='0' y1='0' x2='1' y2='0' style='stroke-width:4px;stroke:red;' vector-effect='non-scaling-stroke' />");
	printf("<line x1='0' y1='0' x2='0' y2='1' style='stroke-width:4px;stroke:green;' vector-effect='non-scaling-stroke' />");
	printf("</g></svg>");
	return 0;
#endif

	// brute-force sampling
	double dx = (x1 - x0) / 24000, dy = (y1 - y0) / 24000;
	int xD = (x1 - x0) / dx, yD = (y1 - y0) / dy;
	long long int count = 0;
	for (int i = 0; i < xD; i++) {
		double x = x0 + i * dx;
		for (int j = 0; j < yD; j++) {
			double y = y0 + j * dy;
			vec2 p(x + .5*dx, y + .5*dy);

			bool ok = false;
			for (int u = 0; u < M; u++) {
				if (isInCapsule(A[u] - p, AB[u], r2)) { ok = true; break; }
			}
			count += ok;
		}
	}

	double Prob = double(count) / (double(xD)*double(yD));
	printf("%lf\n", Area*Prob);
	return 0;
}
