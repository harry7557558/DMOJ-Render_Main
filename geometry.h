// Implement vector/matrix data types - source might not be very readable


#ifndef _INC_GEOMETRY

#define _INC_GEOMETRY

#include <cmath>

#define PI 3.1415926535897932384626
#define mix(x,y,a) ((x)*(1.0-(a))+(y)*(a))
#define clamp(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))
double mod(double x, double m) { return x - m * floor(x / m); }

#ifndef max
#define max(x,y) ((x)>(y)?(x):(y))
#endif
#ifndef min
#define min(x,y) ((x)<(y)?(x):(y))
#endif


class vec2 {
public:
	double x, y;
	explicit vec2() {}
	explicit vec2(const double &a) :x(a), y(a) {}
	explicit vec2(const double &x, const double &y) :x(x), y(y) {}
	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (const vec2 &v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (const vec2 &v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (const vec2 &v) const { return vec2(x * v.x, y * v.y); }	// not standard
	vec2 operator * (const double &a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; } 	// not standard
	friend double length(const vec2 &v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(const vec2 &v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	friend double dot(const vec2 &u, const vec2 &v) { return u.x*v.x + u.y*v.y; }
	friend double det(const vec2 &u, const vec2 &v) { return u.x*v.y - u.y*v.x; } 	// not standard
#if 1
	void operator += (const vec2 &v) { x += v.x, y += v.y; }
	void operator -= (const vec2 &v) { x -= v.x, y -= v.y; }
	void operator *= (const vec2 &v) { x *= v.x, y *= v.y; }
	friend vec2 operator * (const double &a, const vec2 &v) { return vec2(a*v.x, a*v.y); }
	void operator *= (const double &a) { x *= a, y *= a; }
	vec2 operator / (const double &a) const { return vec2(x / a, y / a); }
	void operator /= (const double &a) { x /= a, y /= a; }
#endif
	vec2 yx() const { return vec2(y, x); }
	vec2 rot() const { return vec2(-y, x); }
	vec2 rotr() const { return vec2(y, -x); }
#if 1
	// added when needed
	bool operator == (const vec2 &v) const { return x == v.x && y == v.y; }
	bool operator != (const vec2 &v) const { return x != v.x || y != v.y; }
	vec2 operator / (const vec2 &v) const { return vec2(x / v.x, y / v.y); }
	friend vec2 pMax(const vec2 &a, const vec2 &b) { return vec2(max(a.x, b.x), max(a.y, b.y)); }
	friend vec2 pMin(const vec2 &a, const vec2 &b) { return vec2(min(a.x, b.x), min(a.y, b.y)); }
	friend vec2 abs(const vec2 &a) { return vec2(abs(a.x), abs(a.y)); }
	friend vec2 floor(const vec2 &a) { return vec2(floor(a.x), floor(a.y)); }
	friend vec2 ceil(const vec2 &a) { return vec2(ceil(a.x), ceil(a.y)); }
	friend vec2 sqrt(const vec2 &a) { return vec2(sqrt(a.x), sqrt(a.y)); }
	friend vec2 sin(const vec2 &a) { return vec2(sin(a.x), sin(a.y)); }
	friend vec2 cos(const vec2 &a) { return vec2(cos(a.x), cos(a.y)); }
	friend vec2 atan(const vec2 &a) { return vec2(atan(a.x), atan(a.y)); }
#endif
};



class vec3 {
public:
	double x, y, z;
	explicit vec3() {}
	explicit vec3(const double &a) :x(a), y(a), z(a) {}
	explicit vec3(const double &x, const double &y, const double &z) :x(x), y(y), z(z) {}
	explicit vec3(const vec2 &v, const double &z) :x(v.x), y(v.y), z(z) {}
	vec3 operator - () const { return vec3(-x, -y, -z); }
	vec3 operator + (const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator * (const vec3 &v) const { return vec3(x * v.x, y * v.y, z * v.z); }
	vec3 operator * (const double &k) const { return vec3(k * x, k * y, k * z); }
	double sqr() const { return x * x + y * y + z * z; } 	// non-standard
	friend double length(vec3 v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
	friend vec3 normalize(vec3 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y + v.z*v.z)); }
	friend double dot(vec3 u, vec3 v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
	friend vec3 cross(vec3 u, vec3 v) { return vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
#if 1
	void operator += (const vec3 &v) { x += v.x, y += v.y, z += v.z; }
	void operator -= (const vec3 &v) { x -= v.x, y -= v.y, z -= v.z; }
	void operator *= (const vec3 &v) { x *= v.x, y *= v.y, z *= v.z; }
	friend vec3 operator * (const double &a, const vec3 &v) { return vec3(a*v.x, a*v.y, a*v.z); }
	void operator *= (const double &a) { x *= a, y *= a, z *= a; }
	vec3 operator / (const double &a) const { return vec3(x / a, y / a, z / a); }
	void operator /= (const double &a) { x /= a, y /= a, z /= a; }
#endif
	vec2 xy() const { return vec2(x, y); }
	vec2 xz() const { return vec2(x, z); }
	vec2 yz() const { return vec2(y, z); }
#if 1
	bool operator == (const vec3 &v) const { return x == v.x && y == v.y && z == v.z; }
	bool operator != (const vec3 &v) const { return x != v.x || y != v.y || z != v.z; }
	vec3 operator / (const vec3 &v) const { return vec3(x / v.x, y / v.y, z / v.z); }
	friend vec3 abs(const vec3 &a) { return vec3(abs(a.x), abs(a.y), abs(a.z)); }
	friend vec3 floor(const vec3 &a) { return vec3(floor(a.x), floor(a.y), floor(a.z)); }
	friend vec3 ceil(const vec3 &a) { return vec3(ceil(a.x), ceil(a.y), ceil(a.z)); }
	friend vec3 mod(const vec3 &a, double m) { return vec3(mod(a.x, m), mod(a.y, m), mod(a.z, m)); }
#endif
};




#endif  // _INC_GEOMETRY





#ifndef _INC_SPLINE

#define _INC_SPLINE


// cubic spline class - put it there for some reason

class spline3 {
public:
	vec2 c0, c1, c2, c3;  // c(t) = c3 t³ + c2 t² + c1 t + c0
	spline3() {}
	spline3(vec2 c0, vec2 c1 = vec2(0.), vec2 c2 = vec2(0.), vec2 c3 = vec2(0.)) :c0(c0), c1(c1), c2(c2), c3(c3) {}
	vec2 eval(double t) const {
		return c0 + t * (c1 + t * (c2 + t * c3));
	}
};

spline3 spline3_fromBezier3(vec2 A, vec2 B, vec2 C, vec2 D) {
	return spline3(A, 3.*(B - A), 3.*(A - 2.*B + C), D - 3.*C + 3.*B - A);
}
spline3 spline3_fromBezier2(vec2 A, vec2 B, vec2 C) {
	return spline3(A, 2.*(B - A), A - 2.*B + C);
}
spline3 spline3_fromSegment(vec2 A, vec2 B) {
	return spline3(A, B - A);
}




// range

vec2 getFunctionRange(double c0, double c1, double c2, double c3) {  // cubic polynomial in [0,1]
	vec2 r(c0, c0 + c1 + c2 + c3);
	if (r.x > r.y) { double t = r.x; r.x = r.y; r.y = t; }

	if (c3*c3 < 1e-12) {
		if (c2*c2 < 1e-12) return r;
		double t = -c1 / (2.*c2);
		if (t > 0. && t < 1.) {
			double y = c0 + t * (c1 + t * c2);
			return vec2(min(r.x, y), max(r.y, y));
		}
		return r;
	}

	double a = 3.*c3, b = -c2, c = c1;
	double delta = b * b - a * c;
	if (delta <= 0.) return r;
	delta = sqrt(delta);
	double t = (b - delta) / a;
	if (t > 0. && t < 1.) {
		double y = c0 + t * (c1 + t * (c2 + t * c3));
		r = vec2(min(r.x, y), max(r.y, y));
	}
	t = (b + delta) / a;
	if (t > 0. && t < 1.) {
		double y = c0 + t * (c1 + t * (c2 + t * c3));
		r = vec2(min(r.x, y), max(r.y, y));
	}
	return r;
}
void splineBoundingBox(const spline3 &sp, vec2 &Min, vec2 &Max) {
	vec2 x = getFunctionRange(sp.c0.x, sp.c1.x, sp.c2.x, sp.c3.x);
	vec2 y = getFunctionRange(sp.c0.y, sp.c1.y, sp.c2.y, sp.c3.y);
	Min = vec2(x.x, y.x), Max = vec2(x.y, y.y);
}




// intersection - return shortest positive distance
// text intersection for bounding box before calling this function when performance matters

double splineIntersect(const spline3 &sp, vec2 ro, vec2 rd, int *count = NULL) {
	// cubic equation coefficients
	vec2 n = rd.rot();
	double a = dot(n, sp.c3), b = dot(n, sp.c2), c = dot(n, sp.c1), d = dot(n, sp.c0 - ro);

	// test if a root is valid
	double mt = INFINITY;
	auto testRoot = [&](double t) {
		if (!(t >= 0. && t < 1.)) return;
		t = dot(rd, sp.eval(t) - ro);  // requires rd to be normalized
		if (t > 1e-8) {
			if (count) (*count)++;
			if (t < mt) mt = t;
		}
	};

	// degenerated case
	if (a*a < 1e-16) {
		if (b*b < 1e-16) {
			testRoot(-d / c);
			return mt;
		}
		double delta = c * c - 4.*b*d;
		if (delta > 0.) {
			delta = sqrt(delta);
			testRoot((-c - delta) / (2.*b));
			testRoot((-c + delta) / (2.*b));
		}
		return mt;
	}

	// cubic solver - may have room for optimization
	b /= a, c /= a, d /= a;
	const double _3 = 1. / 3;
	double p = c - _3 * b*b, q = b * (b*b / 13.5 - _3 * c) + d;
	b *= _3, p *= _3, q *= -.5; a = q * q + p * p * p;
	if (a >= 0.) {  // one real root
		a = sqrt(a);
		double t = cbrt(q + a) + cbrt(q - a) - b;
		testRoot(t);
	}
	else {  // three real roots
		c = pow(q*q - a, 1. / 6);
		double u = _3 * atan(sqrt(-a) / q); if (u < 0.) u += PI / 3;
		d = c * sin(u), c *= cos(u);
		testRoot(2.*c - b);
		c = -c, d *= sqrt(3.);
		testRoot(u = c - d - b);
		testRoot(u + 2. * d);
	}
	return mt;
}



#endif  // _INC_SPLINE





// not actually all geometry functions, just some numerical methods

#ifndef _INC_GEOMETRY_MISC

#define _INC_GEOMETRY_MISC


// numerical integration, T can be scalar/vector/matrix
template<typename Fun>
auto NIntegral(Fun f, double a, double b, int n) {
	n *= 2;
	double u = (b - a) / n;
	auto s = f(a)*0.0;
	for (int i = 1; i < n; i += 2) s += f(a + u * i);
	s *= 2.0;
	for (int i = 2; i < n; i += 2) s += f(a + u * i);
	s = 2.0 * s + f(a) + f(b);
	return s * (u / 3.0);
}


// solve a linear system (no handling for degenerated case)
void solveLinear(double* M, double* X, int N) {
	for (int i = 0; i < N; i++) {
		double inv_mi = -1.0 / M[i*N + i];
		for (int j = 0; j < N; j++) if (j != i) {
			double m = inv_mi * M[j*N + i];
			for (int k = i; k < N; k++) M[j*N + k] += M[i*N + k] * m;
			X[j] += X[i] * m;
		}
		for (int j = i; j < N; j++) M[i*N + j] *= -inv_mi;
		X[i] *= -inv_mi;
	}
}


// random number
// override C++ libraries, use it after including C++ libraries to avoid compile error
unsigned _IDUM = 0;
#define srand(seed) do{_IDUM=seed;}while(0)
#define randu (_IDUM=1664525u*_IDUM+1013904223u)  // 32bit unsigned integer
#define randf (randu*(1./4294967295.))   // uniformly between 0 and 1, inclusive

// inverse error function
double erfinv(double x) {
	double n = log(1 - x * x);
	double t = 0.5 * n + 2 / (PI*0.147);
	return (x > 0. ? 1. : -1.) * sqrt(-t + sqrt(t*t - n / 0.147));
}
// normal-distributed random number
inline double randnor(double variance, double median = 0.0) {
	return sqrt(2)*erfinv(2.0 * randf - 1)*variance + median;
}
// random unit vector
inline vec2 randvec2() {
	double t = 2.*PI*randf;
	return vec2(cos(t), sin(t));
}


#endif  // _INC_GEOMETRY_MISC

