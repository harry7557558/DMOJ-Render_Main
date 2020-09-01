#error Header "Geometry.h" include.


// necessary header
#include <cmath>	// or <bits/stdc++.h>

#define PI 3.1415926535897932384626
#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
#define clamp(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))


#pragma region Vector Classes and Operations

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

struct ivec2 {
	int x, y;
	ivec2() {}
	explicit ivec2(int a) :x(a), y(a) {}
	ivec2(int x, int y) :x(x), y(y) {}
	bool operator == (ivec2 v) const { return x == v.x && y == v.y; }
	ivec2 operator - () const { return ivec2(-x, -y); }
	ivec2 operator + (ivec2 v) const { return ivec2(x + v.x, y + v.y); }
	ivec2 operator - (ivec2 v) const { return ivec2(x - v.x, y - v.y); }
	ivec2 operator * (int a) const { return ivec2(x*a, y*a); }
	int sqr() const { return x * x + y * y; }
	friend int dot(ivec2 u, ivec2 v) { return u.x*v.x + u.y*v.y; }
	friend int det(ivec2 u, ivec2 v) { return u.x*v.y - u.y*v.x; }
	ivec2 rot() const { return ivec2(-y, x); }
};

struct vec3 {
	double x, y, z;
	vec3() {}
	explicit vec3(double a) :x(a), y(a), z(a) {}
	vec3(double x, double y, double z) :x(x), y(y), z(z) {}
	vec3 operator - () const { return vec3(-x, -y, -z); }
	vec3 operator + (const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator * (const double &k) const { return vec3(k * x, k * y, k * z); }
	double sqr() { return x * x + y * y + z * z; } 	// non-standard
	friend double length(vec3 v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
	friend vec3 normalize(vec3 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y + v.z*v.z)); }
	friend double dot(vec3 u, vec3 v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
	friend vec3 cross(vec3 u, vec3 v) { return vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
	vec2 xy() const { return vec2(x, y); }
};

template<typename T> T pMax(T p, T q) { return T(max(p.x, q.x), max(p.y, q.y)); }
template<typename T> T pMin(T p, T q) { return T(min(p.x, q.x), min(p.y, q.y)); }




void printVec2(vec2 p, const char* end = "") { printf("(%lg,%lg)%s", p.x, p.y, end); }
void printVec3(vec3 p, const char* end = "") { printf("(%lg,%lg,%lg)%s", p.x, p.y, p.z, end); }
void printTriangle(vec3 A, vec3 B, vec3 C) { printf("Surface(If(u+v<1,"); printVec3(A, "*(1-u-v)+"); printVec3(B, "*u+"); printVec3(C, "*v),u,0,1,v,0,1)\n"); }
void printPolygon(const vec2 *p, int N) { printf("Polyline("); for (int i = 0; i < N; i++) printVec2(p[i], ","); if (N) printVec2(p[0]); printf(")\n"); }
void printTetrahedra(vec3 A, vec3 B, vec3 C, vec3 D) { printf("Polyline("); printVec3(A, ","); printVec3(B, ","); printVec3(C, ","); printVec3(A, ","); printVec3(A, ","); printVec3(C, ","); printVec3(D, ","); printVec3(A, ","); printVec3(A, ","); printVec3(D, ","); printVec3(B, ","); printVec3(A, ")\n"); }
// parametric representation for GeoGebra
// implicit representation for Desmos
void printPoints(const vec2 *p, int N) { printf("["); for (int i = 0; i < N; i++) printVec2(p[i], ","); printf("\b]\n"); }
void printCircle(vec2 c, double r, const char* end = "=0\n") { printf("(x%+lg)^2+(y%+lg)^2-%lg^2%s", -c.x, -c.y, abs(r), end); }
void printCircle_s(vec2 c, double r, const char* end = "=0\n") { printf("\\sqrt{(x%+lg)^2+(y%+lg)^2}-%lg%s", -c.x, -c.y, abs(r), end); }
void printBox(vec2 Min, vec2 Max, const char* end = "=0\n") { printf("max(abs(x%+lg)%+lg,abs(y%+lg)%+lg)%s", -0.5*(Max.x + Min.x), -0.5*(Max.x - Min.x), -0.5*(Max.y + Min.y), -0.5*(Max.y - Min.y), end); }


#pragma endregion



#pragma region Small Useful Functions

// test if segment a and b intersect each other
bool intersect(vec2 ap, vec2 aq, vec2 bp, vec2 bq) {  // change to >0 if collinear is considered intersecting
	if (det(aq - ap, bp - ap) * det(aq - ap, bq - ap) >= 0) return false;
	if (det(bq - bp, ap - bp) * det(bq - bp, aq - bp) >= 0) return false;
	return true;
}
// solve P0 + t0·d0 = P1 + t1·d1
void Intersect(vec2 P0, vec2 P1, vec2 d0, vec2 d1, double &t0, double &t1) {
	vec2 P = P1 - P0;
	double m = 1.0 / det(d1, d0);
	t0 = m * det(d1, P), t1 = m * det(d0, P);
}
// return the point of intersection
vec2 Intersect(vec2 P0, vec2 P1, vec2 d0, vec2 d1) {
	double t = det(d1, P1 - P0) / det(d1, d0);
	return P0 + d0 * t;
}
// 3D intersection
vec2 IntersectXOY(vec3 A, vec3 B) {
	double t = A.z / (A.z - B.z);
	return A.xy()*(1.0 - t) + B.xy()*t;
}


// circle defined by three points, R2 is the SQUARE of the radius
#if 1
void CircsCircle(vec2 p1, vec2 p2, vec2 p3, vec2 &c, double &r2) {
	vec2 d1 = p2 - p1, d2 = p3 - p1;
	vec2 c1 = p1 + d1 * 0.5, c2 = p1 + d2 * 0.5;
	vec2 n1 = d1.rot(), n2 = d2.rot();
	vec2 k = c2 - c1;
	double t = (k.x*n2.y - k.y*n2.x) / (n1.x*n2.y - n2.x*n1.y);
	c = c1 + n1 * t;
	r2 = (c - p1).sqr();
}
#else
void CircsCircle(vec2 P1, vec2 P2, vec2 P3, vec2 &C, double &R2) {
	vec2 a = P2 - P1, b = P3 - P1;
	double a2 = a.sqr(), b2 = b.sqr(), ab = dot(a, b);
	vec2 q = b * a2 - a * b2;
	C = (a * dot(q, b) - b * dot(q, a)) * (0.5 / (a2 * b2 - ab * ab));
	R2 = C.sqr(); C = C + P1;
}
#endif


// test if a point is inside a triangle
bool inTrig(const ivec2 &p, const ivec2 &A, const ivec2 &B, const ivec2 &C) {
	ivec2 a = A - p, b = B - p, c = C - p;
	int ab = det(a, b), bc = det(b, c), ca = det(c, a);
	if (ab == 0 || bc == 0 || ca == 0) return false;	// on the boundary not counted
	return (ab > 0 && bc > 0 && ca > 0) || (ab < 0 && bc < 0 && ca < 0);
}


// 3d rotation
vec3 axis_angle(vec3 v, vec3 axis, double angle) {
	axis = normalize(axis); double ct = cos(angle), st = sin(angle);
	return vec3(
		v.x * (ct + axis.x*axis.x*(1 - ct)) + v.y * (axis.x*axis.y*(1 - ct) - axis.z*st) + v.z * (axis.x*axis.z*(1 - ct) + axis.y*st),
		v.x * (axis.y*axis.x*(1 - ct) + axis.z*st) + v.y * (ct + axis.y*axis.y*(1 - ct)) + v.z * (axis.y*axis.z*(1 - ct) - axis.x*st),
		v.x * (axis.z*axis.x*(1 - ct) - axis.y*st) + v.y * (axis.z*axis.y*(1 - ct) + axis.x*st) + v.z * (ct + axis.z*axis.z*(1 - ct))
	);
}

// distance to a segment (also work for 3d)
double sdSegment(vec2 a, vec2 b, vec2 p) {
	vec2 pa = p - a, ba = b - a;
	double h = dot(pa, ba) / dot(ba, ba);
	if (h < 0.0) h = 0.0; if (h > 1.0) h = 1.0;
	return length(pa - ba * h);		// distance
	return (pa - ba * h).sqr();		// square of distance
}

// fast ray-box intersection (no handling for degenerated cases)
double intBox(vec2 C, vec2 R, vec2 ro, vec2 inv_rd) {  // inv_rd = vec3(1.0)/rd
	vec2 p = -inv_rd * (ro - C);
	vec2 k = vec2(abs(inv_rd.x), abs(inv_rd.y))*R;
	vec2 t1 = p - k, t2 = p + k;
	double tN = max(t1.x, t1.y);
	double tF = min(t2.x, t2.y);
	if (tN > tF + 1e-6 || tF < 0.0) return NAN;  // consider hitting the vertex a intersecting
	return tN;
	//return tN > 0. ? tN : tF;
}


#pragma endregion



#pragma region Computational Geometry Algorithms

// C should be allocated the same size of memory as P
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

// useful for maximum area on convex hull
template<typename Fun> int binarysearchmaximum(Fun F, int x0, int x1, int &x) {
	auto f = [&](int x) { return F(x + 1) - F(x); };
	while (1) {
		x = (x0 + x1) / 2;
		int y = f(x);
		if (y > 0) x0 = x;
		else x1 = x;
		if (x1 - x0 < 2) break;
	}
	if (x1 == x0) { x = x0; return F(x); }
	int y0 = F(x0), y1 = F(x1);
	x = y0 > y1 ? x0 : x1;
	return max(y0, y1);
}




#pragma endregion


