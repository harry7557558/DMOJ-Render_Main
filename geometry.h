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



#endif  // _INC_GEOMETRY





#ifndef _INC_SPLINE

#define _INC_SPLINE


// cubic spline class - put it there for some reason

class spline3 {
public:
	vec2 c0, c1, c2, c3;  // c(t) = c3 t³ + c2 t² + c1 t + c0
	spline3() {}
	spline3(vec2 c0, vec2 c1 = vec2(0.), vec2 c2 = vec2(0.), vec2 c3 = vec2(0.)) :c0(c0), c1(c1), c2(c2), c3(c3) {}

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



#endif  // _INC_SPLINE



