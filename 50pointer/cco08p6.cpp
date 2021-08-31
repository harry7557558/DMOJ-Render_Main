#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;



struct vec2 {
	double x, y;
	explicit vec2() {}
	explicit vec2(const double &a) :x(a), y(a) {}
	explicit vec2(const double &x, const double &y) :x(x), y(y) {}

	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (const vec2 &v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (const vec2 &v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (const vec2 &v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator * (const double &a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; }
	friend double length(const vec2 &v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend double dot(const vec2 &u, const vec2 &v) { return u.x*v.x + u.y*v.y; }
	friend double det(const vec2 &u, const vec2 &v) { return u.x*v.y - u.y*v.x; }

	void operator += (const vec2 &v) { x += v.x, y += v.y; }
	void operator -= (const vec2 &v) { x -= v.x, y -= v.y; }
	void operator *= (const vec2 &v) { x *= v.x, y *= v.y; }
	friend vec2 operator * (const double &a, const vec2 &v) { return vec2(a*v.x, a*v.y); }
	void operator *= (const double &a) { x *= a, y *= a; }
	vec2 operator / (const double &a) const { return vec2(x / a, y / a); }
	void operator /= (const double &a) { x /= a, y /= a; }

	friend vec2 pMax(const vec2 &a, const vec2 &b) { return vec2(max(a.x, b.x), max(a.y, b.y)); }
	friend vec2 pMin(const vec2 &a, const vec2 &b) { return vec2(min(a.x, b.x), min(a.y, b.y)); }

};

struct ivec2 {
	int x, y;
	ivec2() {}
	ivec2(int a) :x(a), y(a) {}
	ivec2(int x, int y) :x(x), y(y) {}
	ivec2(vec2 p);
	bool operator == (const ivec2 &v) const { return x == v.x && y == v.y; }
	bool operator != (const ivec2 &v) const { return x != v.x || y != v.y; }
	ivec2 operator - () const { return ivec2(-x, -y); }
	ivec2 operator + (const ivec2 &v) const { return ivec2(x + v.x, y + v.y); }
	ivec2 operator - (const ivec2 &v) const { return ivec2(x - v.x, y - v.y); }
	ivec2 operator * (const ivec2 &v) const { return ivec2(x * v.x, y * v.y); }
	ivec2 operator * (const int &a) const { return ivec2(x*a, y*a); }
	friend ivec2 operator * (const int &a, const ivec2 &v) { return ivec2(a*v.x, a*v.y); }
	ivec2 operator / (const int &a) const { return ivec2(x / a, y / a); }
	ivec2 operator % (const int &a) const { return ivec2(x % a, y % a); }
	ivec2 yx() const { return ivec2(y, x); }
};

struct ivec3 {
	int x, y, z;
	ivec3() {}
	explicit ivec3(int a) :x(a), y(a), z(a) {}
	explicit ivec3(int x, int y, int z) : x(x), y(y), z(z) {}
	bool operator == (const ivec3 &v) const { return x == v.x && y == v.y && z == v.z; }
	bool operator != (const ivec3 &v) const { return x != v.x || y != v.y || z != v.z; }
	ivec3 operator - () const { return ivec3(-x, -y, -z); }
	ivec3 operator + (const ivec3 &v) const { return ivec3(x + v.x, y + v.y, z + v.z); }
	ivec3 operator - (const ivec3 &v) const { return ivec3(x - v.x, y - v.y, z - v.z); }
	ivec3 operator * (const ivec3 &v) const { return ivec3(x * v.x, y * v.y, z * v.z); }
	ivec3 operator * (const int &a) const { return ivec3(x*a, y*a, z*a); }
	friend ivec3 operator * (const int &a, const ivec3 &v) { return ivec3(a*v.x, a*v.y, a*v.z); }
	ivec3 operator / (const int &a) const { return ivec3(x / a, y / a, z / a); }
	ivec3 operator % (const int &a) const { return ivec3(x % a, y % a, z % a); }
};

struct triangle_2d {
	vec2 v[3];
	vec2& operator[](int d) { return v[d]; }
	const vec2& operator[](int d) const { return v[d]; }
	triangle_2d() {}
	triangle_2d(vec2 v0, vec2 v1, vec2 v2) { v[0] = v0, v[1] = v1, v[2] = v2; }
	triangle_2d(std::initializer_list<vec2> s) { v[0] = *s.begin(), v[1] = *(s.begin() + 1), v[2] = *(s.begin() + 2); }
	vec2 center() const { return (v[0] + v[1] + v[2])*(1. / 3.); }
	void translate(vec2 d) { v[0] += d, v[1] += d, v[2] += d; }
	void scale(double s) { v[0] *= s, v[1] *= s, v[2] *= s; }
	double area() const { return 0.5*det(v[1] - v[0], v[2] - v[0]); }  // signed
};




class Delaunay_2d {

public:

	double epsilon, upsilon;

	triangle_2d supertriangle(const std::vector<vec2> &vertices) {
		vec2 pmin(INFINITY), pmax(-INFINITY);
		for (int i = vertices.size(); i--;) {
			vec2 p = vertices[i];
			pmin = pMin(p, pmin), pmax = pMax(p, pmax);
		}
		vec2 dp = pmax - pmin;
		double d = upsilon * max(dp.x, dp.y);
		vec2 mid = pmin + 0.5*dp;
		vec2 p1 = mid + vec2(0, 1)*d;
		vec2 p2 = mid + vec2(-0.866025, -0.5)*d;
		vec2 p3 = mid + vec2(0.866025, -0.5)*d;
		return triangle_2d(p1, p2, p3);
	}

	struct ccircle {
		int i, j, k;
		vec2 c; double r2;
	};
	ccircle circumcircle(const std::vector<vec2> &vertices, int i, int j, int k) {
		vec2 p1 = vertices[i], p2 = vertices[j], p3 = vertices[k];
		double fabsy1y2 = abs(p1.y - p2.y), fabsy2y3 = abs(p2.y - p3.y);

		vec2 c;

		if (fabsy1y2 < epsilon) {
			double m2 = -((p3.x - p2.x) / (p3.y - p2.y));
			vec2 mp2 = 0.5*(p2 + p3);
			c.x = 0.5*(p1.x + p2.x);
			c.y = m2 * (c.x - mp2.x) + mp2.y;
		}
		else if (fabsy2y3 < epsilon) {
			double m1 = -((p2.x - p1.x) / (p2.y - p1.y));
			vec2 mp1 = 0.5*(p1 + p2);
			c.x = 0.5*(p2.x + p3.x);
			c.y = m1 * (c.x - mp1.x) + mp1.y;
		}

		else {
			double m1 = -((p2.x - p1.x) / (p2.y - p1.y));
			double m2 = -((p3.x - p2.x) / (p3.y - p2.y));
			vec2 mp1 = 0.5*(p1 + p2), mp2 = 0.5*(p2 + p3);
			c.x = (m1 * mp1.x - m2 * mp2.x + mp2.y - mp1.y) / (m1 - m2);
			c.y = fabsy1y2 > fabsy2y3 ?
				m1 * (c.x - mp1.x) + mp1.y :
				m2 * (c.x - mp2.x) + mp2.y;
		}

		return ccircle{ i, j, k, c, (p2 - c).sqr() };
	}

	void delaunay(std::vector<vec2> vertices, std::vector<ivec3> &trigs) {
		trigs.clear();
		int N = (int)vertices.size();
		if (N < 3) return;

		if (N == 3) {
			// imagine you get an empty triangle list in this case
			trigs.push_back(ivec3(0, 1, 2));
			return;
		}

		std::vector<int> indices(N);
		for (int i = 0; i < N; i++) indices[i] = i;
		std::sort(indices.begin(), indices.end(), [&](int i, int j) {
			return vertices[i].x == vertices[j].x ? vertices[i].y > vertices[j].y : vertices[i].x > vertices[j].x;
		});

		triangle_2d st = supertriangle(vertices);
		vertices.push_back(st[0]); vertices.push_back(st[1]); vertices.push_back(st[2]);

		std::vector<ccircle> open, closed;
		open.push_back(circumcircle(vertices, N, N + 1, N + 2));

		for (int i = (int)indices.size(); i--;) {
			std::vector<ivec2> edges;
			int c = indices[i];

			for (int j = (int)open.size(); j--;) {
				// right of the circumcircle
				double dx = vertices[c].x - open[j].c.x;
				if (dx > 0. && dx*dx > open[j].r2) {
					closed.push_back(open[j]);
					open.erase(open.begin() + j);
					continue;
				}
				// outside the circumcircle
				double dy = vertices[c].y - open[j].c.y;
				if (dx * dx + dy * dy - open[j].r2 > epsilon) {
					continue;
				}
				// left of the circumcircle
				edges.push_back(ivec2(open[j].i, open[j].j));
				edges.push_back(ivec2(open[j].j, open[j].k));
				edges.push_back(ivec2(open[j].k, open[j].i));
				open.erase(open.begin() + j);
			}

			// remove duplicate edges
			for (int j = 0; j < (int)edges.size(); j++) {
				for (int i = j + 1; i < (int)edges.size(); i++) {
					if ((edges[j] == edges[i]) || (edges[j].yx() == edges[i])) {
						edges.erase(edges.begin() + i);
						edges.erase(edges.begin() + j);
						j--; break;
					}
				}
			}

			for (int j = edges.size(); j;) {
				ivec2 e = edges[--j];
				open.push_back(circumcircle(vertices, e.x, e.y, c));
			}
		}

		closed.insert(closed.end(), open.begin(), open.end());
		open.clear();

		trigs.clear();
		for (int i = closed.size(); i--;) {
			if (closed[i].i < N && closed[i].j < N && closed[i].k < N) {
				trigs.push_back(ivec3(closed[i].i, closed[i].j, closed[i].k));
			}
		}
	}


	Delaunay_2d(double epsilon = (double)1e-6, double upsilon = (double)1e+4)
		: epsilon(epsilon), upsilon(upsilon) {}

};




int N;
std::vector<vec2> P;

#define MAXI 1<<22
char _i[MAXI], _, __; int i0 = 0;
#define scanu(x) do{while((x=_i[i0++])<'0');for(x-='0';'0'<=(_=_i[i0++]);x=x*10+_-'0');}while(0)
#define scan(x) do{while((__=_i[i0++])<45);if(__-45)x=__;else x=_i[i0++];for(x-=48;47<(_=_i[i0++]);x=x*10+_-48);if(__<46)x=-x;}while(0)


int main() {
#ifdef __DEBUG
	const std::string test_case = "F.15";
	freopen(&("cco08p6/" + test_case + ".correct")[0], "r", stdin);
	string solution; cin >> solution; cout << solution << endl;
	freopen(&("cco08p6/" + test_case + ".dat")[0], "r", stdin);
#endif
	_i[fread(_i, 1, MAXI, stdin)] = 0;

	scanu(N); P.resize(N);
	for (int i = 0; i < N; i++) {
		int pi; scan(pi); P[i].x = pi;
		scan(pi); P[i].y = pi;
	}

	auto Delaunay = Delaunay_2d();

	std::vector<ivec3> T;
	Delaunay.delaunay(P, T);

	double maxr2 = 0.;
	for (int i = 0; i < (int)T.size(); i++) {
		Delaunay_2d::ccircle c = Delaunay.circumcircle(P, T[i].x, T[i].y, T[i].z);
		maxr2 = max(maxr2, c.r2);
	}
	printf("%lf\n", sqrt(maxr2));



	return 0;
}
