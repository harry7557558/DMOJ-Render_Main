#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>


typedef long double ldouble;

struct vec2 {
	ldouble x, y;
	vec2() {}
	explicit vec2(ldouble a) :x(a), y(a) {}
	vec2(ldouble x, ldouble y) :x(x), y(y) {}
	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (vec2 v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (vec2 v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (vec2 v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator * (ldouble a) const { return vec2(x*a, y*a); }
	ldouble sqr() const { return x * x + y * y; }
	friend ldouble length(vec2 v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(vec2 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	friend ldouble dot(vec2 u, vec2 v) { return u.x*v.x + u.y*v.y; }
	friend ldouble det(vec2 u, vec2 v) { return u.x*v.y - u.y*v.x; }
	vec2 rot() const { return vec2(-y, x); }
};

typedef std::vector<vec2> polygon;

// cut dot(p,n)>c, assume fig is convex
polygon cutFigure(const polygon &fig, vec2 n, ldouble c) {

	//printf("%lg*x%+lg*y>%lg\n", n.x, n.y, c);

	int l = fig.size();
	polygon res;

	// find a point that inside the polygon
	int d0;
	for (d0 = 0; d0 < l; d0++) {
		if (dot(fig[d0], n) < c) break;
	}
	if (d0 >= l) return res;

	// trace segment
	auto intersect = [](vec2 p, vec2 q, const ldouble &c, const vec2 &n)->vec2 {
		q = q - p;
		ldouble t = (c - dot(n, p)) / dot(n, q);
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




int N;
ldouble a[100000], b[100000], c[100000], r[100000];
vec2 n[100000];


int countPassed(vec2 p) {
	if (p.x > 0 || p.y < 0) return 0;
	int passed_count = 0;
	for (int i = 0; i < N; i++) {
		ldouble u = dot(n[i], p);
		if (abs(u - c[i]) - r[i] < 1e-8) passed_count++;
		else break;
	}
	return passed_count;
}


vec2 intersect(vec2 n1, ldouble c1, vec2 n2, ldouble c2) {
	ldouble invdet = 1. / det(n1, n2);
	return vec2(c1*n2.y - c2 * n1.y, n1.x*c2 - n2.x * c1) * invdet;
}



int main() {
#ifdef __DEBUG
	freopen("stdin.dat", "r", stdin);
#endif

	std::cin >> N;
	for (int i = 0; i < N; i++) {
		ldouble x, y1, y2; std::cin >> x >> y1 >> y2;
		n[i] = vec2(x*x, x);
		a[i] = y1, b[i] = y2;
		c[i] = 0.5*(y1 + y2);
		r[i] = 0.5*abs(y1 - y2);
		//printf("abs(%lg*x%+lg*y%+lg)<%lg\n", n[i].x, n[i].y, -c[i], r[i]);
	}


#if 0

	// bruteforce reference solution
	int maxpassed = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			ldouble x, y;
			maxpassed = std::max(countPassed(intersect(n[i], a[i], n[j], a[j])), maxpassed);
			maxpassed = std::max(countPassed(intersect(n[i], a[i], n[j], b[j])), maxpassed);
			maxpassed = std::max(countPassed(intersect(n[i], b[i], n[j], a[j])), maxpassed);
			maxpassed = std::max(countPassed(intersect(n[i], b[i], n[j], b[j])), maxpassed);
		}
	}
	return 0 * printf("%d\n", maxpassed);

#else

	polygon s({ vec2(-1e9, -1e9), vec2(1e9, 0), vec2(0, 1e9) });

	for (int i = 0; i < N; i++) {
		s = cutFigure(s, n[i], b[i] + 1e-12);
		s = cutFigure(s, -n[i], -a[i] + 1e-12);
		if (s.empty()) {
			printf("%d\n", i);
			return 0;
		}

		/*printf("polygon(");
		for (int i = 0; i < (int)s.size(); i++) {
		printf("(%lg,%lg),", s[i].x, s[i].y);
		}
		printf("\b)\n");*/
	}
	return 0 * printf("%d\n", N);


#endif


}