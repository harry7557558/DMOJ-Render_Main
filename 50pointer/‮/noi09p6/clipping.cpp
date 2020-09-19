#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;


// 2d vector template
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


#define EPSILON 1e-8
#define UPSILON 1e+8
#define PI 3.1415926535897932384626
#define NAV vec2(NAN)


// capsule structure
struct capsule {
	vec2 a, b;
	static double r, r2;
};
double capsule::r = 0;
double capsule::r2 = 0;

// line segment structure
struct segment {
	vec2 a, b;
	bool isIn(vec2 d) const {
		vec2 ab = b - a, ad = d - a;
		double h = dot(ad, ab) / ab.sqr();
		return h > 0. && h < 1.;
	}
};

// circular arc structure
struct arc {
	vec2 c;  // center
	vec2 d1, d2;  // starting and ending vectors with length equal to capsule::r, counter-clockwise
	// a valid arc should have det(d1,d2)>0; vector d satisfying det(d1,d)>0&&det(d,d2)>0 belongs to the arc
	bool isIn(vec2 d) const {
		if (!(det(d1, d2) > 0.)) return false;
		d = d - c;
		return det(d1, d) > 0. && det(d, d2) > 0.;
	}
};


// intersection between line segments
vec2 segmentInt(segment s1, segment s2) {
	vec2 p1 = s1.a, d1 = s1.b - p1, p2 = s2.a, d2 = s2.b - p2;
	double Det = det(d1, d2);
	if (abs(Det) < EPSILON) {  // degenerated case
		return NAV;  // may cause problems
	}
	double u = det(p2 - p1, d2) / Det;
	if (u<0. || u>1.) return NAV;  // may cause problems
	double v = -det(d1, p2 - p1) / Det;
	if (v<0. || v>1.) return NAV;  // may cause problems
	return p1 + d1 * u;
}

// intersection between arcs
vec2s arcInt(arc a1, arc a2) {
	vec2 c1 = a1.c, c2 = a2.c, c = (c1 + c2)*.5;
	vec2 d = c2 - c1, dr = normalize(d.rot());
	double l = capsule::r2 - .25*d.sqr();
	if (l < 0.) return vec2s(NAV, NAV);  // should not cause precision problems
	else l = sqrt(l);
	vec2 p1 = c + dr * l, p2 = c - dr * l;
	if (!(a1.isIn(p1) && a2.isIn(p1))) p1 = NAV;  // may cause problems
	if (!(a1.isIn(p2) && a2.isIn(p2))) p2 = NAV;  // may cause problems
	return vec2s(p1, p2);
}

// intersection between a segment and an arc
vec2s segarcInt(segment s_, arc a_) {
	vec2 a = s_.a, b = s_.b, c = a_.c;
	vec2 d = b - a, p = a - c;
	double d2 = d.sqr(), dp = dot(d, p);
	double delta = dp * dp - d2 * (p.sqr() - capsule::r2);
	if (delta < 0.) return vec2s(NAV, NAV);
	delta = sqrt(delta) / d2, dp /= d2;
	double t0 = delta - dp; vec2 p0 = (t0 > 0. && t0 < 1.) ? a + d * t0 : NAV;
	double t1 = -delta - dp; vec2 p1 = (t1 > 0. && t1 < 1.) ? a + d * t1 : NAV;
	if (!a_.isIn(p0)) p0 = NAV;
	if (!a_.isIn(p1)) p1 = NAV;
	return vec2s(p0, p1);
}



// don't like to use OO
struct section {
	bool isArc;  // false: segment; true: arc
	segment s;
	arc a;
};
typedef vector<section> path;

// intersection between two sections
vec2s sectionInt(section s1, section s2) {
	if (s1.isArc) {
		if (s2.isArc) return arcInt(s1.a, s2.a);
		return segarcInt(s2.s, s1.a);
	}
	if (s2.isArc) return segarcInt(s1.s, s2.a);
	return vec2s(segmentInt(s1.s, s2.s), NAV);
}


// calculate the area enclosed by a path
// a valid path should not have a negative area
double calcPathArea(path p) {
	return NAN;
}

// debug output, visualize path using Desmos graphing calculator
void printPath(path p) {
	printf("{var s=Calc.getState();s['expressions']['list'].push(");
	static int id = 0;
	for (int i = 0; i < p.size(); i++) {
		char latex[256];
		double t0 = 0, t1 = 1;
		if (p[i].isArc) {
			vec2 c = p[i].a.c; double r = capsule::r;
			vec2 d0 = p[i].a.d1, d1 = p[i].a.d2;
			t0 = atan2(d0.y, d0.x), t1 = atan2(d1.y, d1.x); if (t1 < t0) t1 += 2 * PI;
			sprintf(latex, "(%lg+%lg\\\\cos(t),%lg+%lg\\\\sin(t))", c.x, r, c.y, r);
		}
		else {
			vec2 a = p[i].s.a, b = p[i].s.b;
			sprintf(latex, "(%lg%+lgt,%lg%+lgt)", a.x, b.x - a.x, a.y, b.y - a.y);
		}
		printf("{type:'expression',id:'_%d',latex:'%s',parametricDomain:{min:'%lg',max:'%lg'},color:'#000'},", ++id, latex, t0, t1);
	}
	printf(");Calc.setState(s);}\n");
}



// convert a capsule to a path
path toPath(capsule cp) {
	vec2 a = cp.a, b = cp.b;
	vec2 d = b - a, dr = normalize(d.rot())*cp.r;
	vector<section> path;
	section tmp;
	tmp.isArc = false;
	tmp.s = segment{ a - dr, b - dr };
	path.push_back(tmp);
	tmp.isArc = true;
	tmp.a = arc{ b, -dr, dr };
	path.push_back(tmp);
	tmp.isArc = false;
	tmp.s = segment{ b + dr, a + dr };
	path.push_back(tmp);
	tmp.isArc = true;
	tmp.a = arc{ a, dr, -dr };
	path.push_back(tmp);
	return path;
}

// clipping
path clipCapsule(const path &pt, capsule cp) {
	section temp;
	path s = toPath(cp);
	printPath(s);
	if (pt.empty()) return s;

	path pt1 = s;
	//printPath(s);
	return pt1;
}



// data as global variable
int T, N;
vec2 Pnt[1000];
int M;
capsule Cap[1000];


int main() {

	// input
	cin >> T >> N;
	for (int i = 0; i < N; i++) cin >> Pnt[i].x >> Pnt[i].y;
	cin >> M;
	for (int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		Cap[i].a = Pnt[a], Cap[i].b = Pnt[b];
	}
	cin >> capsule::r;
	capsule::r2 = capsule::r*capsule::r;

	path pt;
	for (int i = 0; i < M; i++) {
		pt = clipCapsule(pt, Cap[i]);
	}

	{
		segment s1{ vec2(2.65,0.97), vec2(-0.42,0.85) };
		segment s2{ vec2(-0.44,6.35), vec2(3.19,2.69) };
		double t10 = 0.4, t11 = 1.7; vec2 c1(0.58, 3.62); arc a1 = arc{ c1, vec2(cos(t10),sin(t10))*capsule::r, vec2(cos(t11),sin(t11))*capsule::r };
		double t20 = 5.3, t21 = 5.8; vec2 c2(-0.43, 6.75); arc a2 = arc{ c2, vec2(cos(t20),sin(t20))*capsule::r, vec2(cos(t21),sin(t21))*capsule::r };
		vec2 p = arcInt(a1, a2).first;
		vec2 q = arcInt(a1, a2).second;
		printf("(%lf,%lf)\n(%lf,%lf)\n", p.x, p.y, q.x, q.y);
	}

	// clipping

	return 0;
}

