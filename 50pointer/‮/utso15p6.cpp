// A copy-paste of harry7557558's first AC submission

// Add this line:
#pragma GCC optimize "Os"


#include <bits/stdc++.h>
using namespace std;

// GLSL style vector classes
class vec2 {
public:
	double x, y;
	vec2() {}
	vec2(double a) :x(a), y(a) {}
	vec2(double x, double y) :x(x), y(y) {}
	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (vec2 v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (vec2 v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (double a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; }
	friend double length(vec2 v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(vec2 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	friend double dot(vec2 u, vec2 v) { return u.x*v.x + u.y*v.y; }
	friend double det(vec2 u, vec2 v) { return u.x*v.y - u.y*v.x; }
	vec2 rot() const { return vec2(-y, x); }  // rotate 90deg counterclockwise
};
class vec3 {
public:
	double x, y, z;
	vec3() {}
	vec3(double a) :x(a), y(a), z(a) {}
	vec3(double x, double y, double z) :x(x), y(y), z(z) {}
	vec3 operator - () const { return vec3(-x, -y, -z); }
	vec3 operator + (const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator * (const double &k) const { return vec3(k * x, k * y, k * z); }
	double sqr() { return x * x + y * y + z * z; }
	friend double length(vec3 v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
	friend vec3 normalize(vec3 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y + v.z*v.z)); }
	friend double dot(vec3 u, vec3 v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
	friend vec3 cross(vec3 u, vec3 v) { return vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
	vec2 xy() const { return vec2(x, y); }
};

class mat3 {
public:
	double _p[3][3];
	mat3() {}
	mat3(double d) { for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) _p[i][j] = d * (i == j); }
	mat3(vec3 a, vec3 b, vec3 c) {
		_p[0][0] = a.x, _p[1][0] = a.y, _p[2][0] = a.z;
		_p[0][1] = b.x, _p[1][1] = b.y, _p[2][1] = b.z;
		_p[0][2] = c.x, _p[1][2] = c.y, _p[2][2] = c.z;
	}
	double det() const {
		return _p[0][0] * (_p[1][1] * _p[2][2] - _p[1][2] * _p[2][1]) - _p[0][1] * (_p[1][0] * _p[2][2] - _p[1][2] * _p[2][0]) + _p[0][2] * (_p[1][0] * _p[2][1] - _p[1][1] * _p[2][0]);
	}
	mat3 invert() const {
		mat3 R;
		R._p[0][0] = _p[1][1] * _p[2][2] - _p[1][2] * _p[2][1], R._p[0][1] = _p[0][2] * _p[2][1] - _p[0][1] * _p[2][2], R._p[0][2] = _p[0][1] * _p[1][2] - _p[0][2] * _p[1][1];
		R._p[1][0] = _p[1][2] * _p[2][0] - _p[1][0] * _p[2][2], R._p[1][1] = _p[0][0] * _p[2][2] - _p[0][2] * _p[2][0], R._p[1][2] = _p[0][2] * _p[1][0] - _p[0][0] * _p[1][2];
		R._p[2][0] = _p[1][0] * _p[2][1] - _p[1][1] * _p[2][0], R._p[2][1] = _p[0][1] * _p[2][0] - _p[0][0] * _p[2][1], R._p[2][2] = _p[0][0] * _p[1][1] - _p[0][1] * _p[1][0];
		double d = 1.0 / (_p[0][0] * R._p[0][0] + _p[0][1] * R._p[1][0] + _p[0][2] * R._p[2][0]);
		for (int i = 0; i < 9; i++) ((double*)&R._p[0][0])[i] *= d;
		return R;
	}
	vec3 operator * (const vec3 &p) const {
		return vec3(
			_p[0][0] * p.x + _p[0][1] * p.y + _p[0][2] * p.z,
			_p[1][0] * p.x + _p[1][1] * p.y + _p[1][2] * p.z,
			_p[2][0] * p.x + _p[2][1] * p.y + _p[2][2] * p.z
		);
	}
};

struct tetrahedron { vec3 P[4]; };
struct triangle3D { vec3 A, B, C; };
struct triangle { vec2 A, B, C; };
typedef std::vector<vec2> polygon;

// debug
void printVec2(const vec2 &p, const char* end = "") { printf("(%lg,%lg)%s", p.x, p.y, end); }
void printVec3(const vec3 &p, const char* end = "") { printf("(%lg,%lg,%lg)%s", p.x, p.y, p.z, end); }
void printTriangle(const triangle3D &t) {
	printf("Surface(If(u+v<1,");
	printVec3(t.A, "+"); printVec3(t.B - t.A, "*u+"); printVec3(t.C - t.A, "*v");
	printf("),u,0,1,v,0,1)\n");
}
void printTetrahedron(const tetrahedron &t) {
	printf("Polyline(");
	printVec3(t.P[0], ",");
	for (int i = 1; i < 4; i++)
		printVec3(t.P[i], ","), printVec3(t.P[i % 3 + 1], ","), printVec3(t.P[0], ",");
	printf("\b)\n");
}
void printPolygon(const polygon &p) {
	printf("Polyline(");
	for (int i = 0; i < p.size(); i++) printVec2(p[i], ",");
	printVec2(p[0], ")\n");
}
void printPolygon3D(const polygon &p, mat3 M = mat3(1.0), vec3 T = vec3(0.0)) {
	printf("Polyline(");
	for (int i = 0; i < p.size(); i++) printVec3(M*vec3(p[i].x, p[i].y, 0) + T, ",");
	printVec3(M*vec3(p[0].x, p[0].y, 0) + T, ")\n");
}
ostream& operator << (ostream& os, const vec2 &p) { os << "(" << p.x << "," << p.y << ")"; return os; }
ostream& operator << (ostream& os, const vec3 &p) { os << "(" << p.x << "," << p.y << "," << p.z << ")"; return os; }
istream& operator >> (istream& is, vec3 &p) { is >> p.x >> p.y >> p.z; return is; }


double triangleArea(vec3 A, vec3 B, vec3 C) { return 0.5*length(cross(B - A, C - A)); }
double triangleArea(vec2 A, vec2 B, vec2 C) { return 0.5*abs(det(B - A, C - A)); }



// cut dot(p-p0,n)>0
polygon cutFigure(const polygon &fig, vec2 p0, vec2 n) {
	const double c = dot(p0, n);	// dot(p,n)>c
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

// test if segment a and b intersect each other
bool intersect(vec2 ap, vec2 aq, vec2 bp, vec2 bq) {  // change to >0 if collinear is considered intersecting
	if (det(aq - ap, bp - ap) * det(aq - ap, bq - ap) >= 0) return false;
	if (det(bq - bp, ap - bp) * det(bq - bp, aq - bp) >= 0) return false;
	return true;
}




// return true if the tetrahedron (possibly) intersects the triangle
bool isPossibleIntersecting(const tetrahedron &TH, const triangle3D &T) {
	// plane of the triangle: dot(N,P) = d
	vec3 O = T.A, a = T.B - O, b = T.C - O;  // T = O + u a + v b, 0<u,v<1, u+v<1
	vec3 N = cross(a, b);
	double d = dot(N, O);
	// D: "signed distance" to from the vertices to the plane, non-zero as specified
	double D[4];
	for (int i = 0; i < 4; i++) D[i] = dot(TH.P[i], N) - d;
	if (D[1] * D[0] > 0 && D[2] * D[0] > 0 && D[3] * D[0] > 0) return false;  // on the same side of the plane
	return true;

	// test all edges
	for (int i = 0; i < 4; i++) for (int j = i + 1; j < 4; j++) {
		if (D[i] * D[j] < 0) {  // intersect the place
			vec3 P = TH.P[i], S = TH.P[j] - P, R = P - O;  // P + t S, 0<t<1
			double invdet = 1.0 / dot(S, N);
			double t = dot(R, N) * invdet;  // seems positive
			if (t < 0.0) {
				cout << "BUG!\n";
			}
			if (t < 1.0) {
				double u = dot(R, cross(S, b)) * invdet;
				if (u > 0 && u < 1) {
					double v = dot(R, cross(a, S)) * invdet;
					if (v > 0 && u + v < 1) {
						return true;
					}
				}
			}
		}
	}
}

// return the area of the triangle that is outside the tetrahedron
double intersectArea(const tetrahedron &TH, const triangle3D &T) {
	//if (!isPossibleIntersecting(TH, T)) return triangleArea(T.A, T.B, T.C);
	vec3 O = T.C, A = T.A - O, B = T.B - O, N = cross(A, B);
	mat3 M = mat3(A, B, N).invert();
	double ScF = length(N);
	vec3 D[4];
	for (int i = 0; i < 4; i++) D[i] = M * (TH.P[i] - O);
	A = M * A, B = M * B;
	double Area = triangleArea(vec3(0.0), A, B);  // 0.5
	int PC = (D[0].z > 0) + (D[1].z > 0) + (D[2].z > 0) + (D[3].z > 0);  // number of positive zs
	if (PC % 4 == 0) {  // no intersection
		return Area * ScF;
	}
	//printf("\n"); printTetrahedron(TH); printTriangle(T);
	auto intXOY = [](vec3 P, vec3 Q) -> vec2 {
		double t = P.z / (P.z - Q.z);
		return P.xy()*(1.0 - t) + Q.xy()*t;
	};
	auto calcClippedArea = [](polygon Fig, vec2 A0, vec2 B0, vec2 C0) {
		if (det(A0 - C0, B0 - C0) < 0) swap(A0, B0);
		Fig = cutFigure(Fig, A0, (A0 - B0).rot());
		Fig = cutFigure(Fig, B0, (B0 - C0).rot());
		Fig = cutFigure(Fig, C0, (C0 - A0).rot());
		return calcArea(Fig);
	};
	if (PC == 1 || PC == 3) {
		vec3 C, CA, CB, CC;
		if (PC == 1) {
			if (D[0].z > 0) C = D[0], CA = D[1], CB = D[2], CC = D[3];
			else if (D[1].z > 0)  C = D[1], CA = D[0], CB = D[2], CC = D[3];
			else if (D[2].z > 0) C = D[2], CA = D[0], CB = D[1], CC = D[3];
			else C = D[3], CA = D[0], CB = D[1], CC = D[2];
		}
		else if (PC == 3) {
			if (D[0].z <= 0) C = D[0], CA = D[1], CB = D[2], CC = D[3];
			else if (D[1].z <= 0)  C = D[1], CA = D[0], CB = D[2], CC = D[3];
			else if (D[2].z <= 0) C = D[2], CA = D[0], CB = D[1], CC = D[3];
			else C = D[3], CA = D[0], CB = D[1], CC = D[2];
		}
		vec2 A0 = intXOY(C, CA), B0 = intXOY(C, CB), C0 = intXOY(C, CC);
		double At = calcClippedArea(polygon({ vec2(0.0), A.xy(), B.xy() }), A0, B0, C0);
		return (Area - At) * ScF;
	}
	if (PC == 2) {
		polygon Fig({ vec2(0.0), A.xy(), B.xy() });
		vec3 CA, CB, DA, DB;
		if (D[0].z < 0) {
			CA = D[0];
			if (D[1].z < 0) CB = D[1], DA = D[2], DB = D[3];
			else if (D[2].z < 0) CB = D[2], DA = D[1], DB = D[3];
			else CB = D[3], DA = D[1], DB = D[2];
		}
		else if (D[1].z < 0) {
			CA = D[1], DA = D[0];
			if (D[2].z < 0) CB = D[2], DB = D[3];
			else CB = D[3], DB = D[2];
		}
		else CA = D[2], CB = D[3], DA = D[0], DB = D[1];
		vec2 B0 = intXOY(CA, DA), B1 = intXOY(CA, DB), B2 = intXOY(CB, DA), B3 = intXOY(CB, DB);  // convex?
		if (intersect(B0, B1, B2, B3)) swap(B0, B3);
		else if (intersect(B0, B3, B1, B2)) swap(B0, B1);
		//printPolygon(Fig); printPolygon(polygon({ B0, B1, B2, B3 }));
		double At1 = calcClippedArea(polygon({ vec2(0.0), A.xy(), B.xy() }), B0, B2, B1);
		double At2 = calcClippedArea(polygon({ vec2(0.0), A.xy(), B.xy() }), B0, B2, B3);
		//printPolygon3D(Fig, M.invert(), O);
		return (Area - (At1 + At2)) * ScF;
	}
	return NAN;
}

#ifdef _DEBUG
ifstream fin("stdin.dat");
#define cin fin
#endif

int main() {
	tetrahedron A, B;
	for (int i = 0; i < 4; i++) cin >> A.P[i].x >> A.P[i].y >> A.P[i].z;
	for (int i = 0; i < 4; i++) cin >> B.P[i].x >> B.P[i].y >> B.P[i].z;
	//printf("A="); printTetrahedron(A); printf("B="); printTetrahedron(B);
	double Area = 0;
	for (int i = 0; i < 4; i++) for (int j = i + 1; j < 4; j++) for (int k = j + 1; k < 4; k++) {
		Area += intersectArea(B, triangle3D{ A.P[i], A.P[j], A.P[k] });
		Area += intersectArea(A, triangle3D{ B.P[i], B.P[j], B.P[k] });
	}
	printf("%.8lf\n", Area);
	return 0;
}