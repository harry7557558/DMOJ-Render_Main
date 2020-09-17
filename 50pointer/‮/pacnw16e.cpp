// ICPC PACNW 2016 E - Enclosure

#pragma GCC optimize "Ofast"
#pragma GCC optimize "unroll-loops"
#pragma GCC target "sse,sse2,sse3,sse4,abm,avx,mmx,popcnt"
#include <bits/stdc++.h>
using namespace std;

char _, __;
#define scanu(x) do{while((x=getchar())<'0');for(x-='0';'0'<=(_=getchar());x=x*10+_-'0');}while(0)
#define scan(x) do{while((__=getchar())<45);if(__-45)x=__;else x=getchar();for(x-=48;47<(_=getchar());x=x*10+_-48);if(__<46)x=-x;}while(0)


typedef long long lint;
#define INF 0x7FFFFFFFFFFFFFFF


// 2D vector template
class ivec2 {
public:
	lint x, y;
	ivec2() {}
	ivec2(lint a) :x(a), y(a) {}
	ivec2(lint x, lint y) :x(x), y(y) {}
	bool operator == (ivec2 v) const { return x == v.x && y == v.y; }
	ivec2 operator - () const { return ivec2(-x, -y); }
	ivec2 operator + (ivec2 v) const { return ivec2(x + v.x, y + v.y); }
	ivec2 operator - (ivec2 v) const { return ivec2(x - v.x, y - v.y); }
	ivec2 operator * (lint a) const { return ivec2(x*a, y*a); }
	lint sqr() const { return x * x + y * y; }
	friend lint dot(ivec2 u, ivec2 v) { return u.x*v.x + u.y*v.y; }
	friend lint det(ivec2 u, ivec2 v) { return u.x*v.y - u.y*v.x; }
	ivec2 rot() const { return ivec2(-y, x); }  // rotate 90deg counterclockwise
};

#define clamp(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))




#define MAXN 100000
ivec2 P[MAXN]; int N, K;
ivec2 C[MAXN]; int Cn = 0;  // convex hull
lint Ps[MAXN];  // prefix sum array for area



// acceleration structures

struct Segment {
	ivec2 p, q;  // vertexes
	int id;  // index on the convex hull
} S0[MAXN], S[MAXN];  // S0 is constant while S will be modified by sorting

// In an intersection test, the ray is considered intersecting the Segment if it hits p exactly, but not q

// shoot a ray toward right
bool intSegment(const Segment &s, ivec2 ro) {
	ivec2 p = s.p, q = s.q;
	if (p.y < q.y) {
		if (det(p - ro, q - ro) <= 0);
		else if (ro.y >= p.y && ro.y < q.y) return 1;
	}
	else if (p.y > q.y) {
		if (det(p - ro, q - ro) >= 0);
		else if (ro.y <= p.y && ro.y > q.y) return 1;
	}
	else {
		if (ro.y != p.y);
		else if (ro.x < p.x) return 1;
	}
	return 0;
}

// return the minimum positive horizontal distance to a segment
double intSegmentD(const Segment &s, ivec2 ro) {
	ivec2 p = s.p, q = s.q;
	if (p.y < q.y && (ro.y < p.y || ro.y >= q.y)) return NAN;
	if (p.y > q.y && (ro.y > p.y || ro.y <= q.y)) return NAN;
	double t = (ro.y - p.y) / double(q.y - p.y);
	return abs(p.x + t * (q.x - p.x) - ro.x);
}

// node of the tree
// not sure what this tree is called, it is a simplified version of BVH structure used in ray-tracing
struct Node {
	lint Min, Max;  // range on y-axis
	Segment s;  // valid if children are nullptr
	Node *c1 = 0, *c2 = 0;
} R;

// recursively construct a binary search tree
// P is already sorted by y
void constructTree(Node *R, Segment *P, int N) {
	if (N == 1) {
		R->s = P[0];
		R->Max = max(P[0].p.y, P[0].q.y);
		R->Min = min(P[0].p.y, P[0].q.y);
	}
	else {
		R->c1 = new Node; constructTree(R->c1, P, N / 2);
		R->c2 = new Node; constructTree(R->c2, P + N / 2, N - N / 2);
		R->Max = max(R->c1->Max, R->c2->Max);
		R->Min = min(R->c1->Min, R->c2->Min);
	}
}

// shoot a ray toward right, count the number of intersections
int intersectTree(Node *R, ivec2 ro) {
	if (R->c1 || R->c2) {
		int n1 = 0, n2 = 0;
		if (ro.y >= R->c1->Min && ro.y <= R->c1->Max)
			n1 = intersectTree(R->c1, ro);
		if (ro.y >= R->c2->Min && ro.y <= R->c2->Max)
			n2 = intersectTree(R->c2, ro);
		return n1 + n2;
	}
	else {
		return intSegment(R->s, ro);
	}
}

// get the index of the (horizontally) closest point
int getClosestIdY(Node *R, ivec2 ro) {
	if (R->c1 || R->c2) {
		int i1 = -1, i2 = -1;
		if (ro.y >= R->c1->Min && ro.y <= R->c1->Max)
			i1 = getClosestIdY(R->c1, ro);
		if (ro.y >= R->c2->Min && ro.y <= R->c2->Max)
			i2 = getClosestIdY(R->c2, ro);
		if (i1 == -1) return i2;
		if (i2 == -1) return i1;
		if (i1 == -1 && i2 == -1) return -1;
		return intSegmentD(S0[i1], ro) < intSegmentD(S0[i2], ro) ? i1 : i2;
	}
	else {
		return R->s.id;
	}
}




int main() {
#ifdef __DEBUG
	freopen("stdin.dat", "r", stdin);
#endif
	scanu(N); scanu(K);
	for (int i = 0; i < N; i++) {
		int t; scan(t); P[i].x = t;
		scan(t); P[i].y = t;
	}
	sort(P, P + K, [](ivec2 p, ivec2 q) { return p.x == q.x ? p.y < q.y : p.x < q.x; });

	// calculate current "territory" (convex hull)
	C[Cn++] = P[0];
	for (int i = 1; i < K;) {
		if (Cn == 1) C[Cn++] = P[i];
		else {
			if (det(C[Cn - 1] - C[Cn - 2], P[i] - C[Cn - 2]) <= 0) {
				C[Cn - 1] = P[i];
				while (Cn > 2 && det(C[Cn - 2] - C[Cn - 3], C[Cn - 1] - C[Cn - 3]) <= 0) Cn--, C[Cn - 1] = P[i];
			}
			else C[Cn++] = P[i];
		}
		do { i++; } while (i < K && P[i].x == P[i - 1].x);
	}
	for (int i = K - 1; i >= 0;) {
		if (i == K - 1) {
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

	// calculate (2x) current "power"
	lint totA = 0;
	for (int i = 0; i < Cn; i++) {
		lint dA = det(C[i], C[(i + 1) % Cn]);
		totA += dA;
		Ps[i] = totA;
	}

	// calculate the bounding box of the current "territory"
	ivec2 RMin = ivec2(INF), RMax = -RMin;
	int tId = -1, bId = -1;  // topmost and bottommost point
	for (int i = 0; i < Cn; i++) {
		if (C[i].x < RMin.x) RMin.x = C[i].x;
		if (C[i].y < RMin.y) RMin.y = C[i].y, bId = i;
		if (C[i].x > RMax.x) RMax.x = C[i].x;
		if (C[i].y > RMax.y) RMax.y = C[i].y, tId = i;
	}

	// initialize acceleration structure
	for (int i = 0; i < Cn; i++) {
		S0[i].p = C[i];
		S0[i].q = C[(i + 1) % Cn];
		S0[i].id = i;
		S[i] = S0[i];
	}
	std::sort(S, S + Cn, [](Segment p, Segment q) { return p.p.y < q.p.y; });  // sort by p.y
	constructTree(&R, S, Cn);

	// loop through all "trees"
	lint maxA = 0;  // maximum gained area
	for (int i = K; i < N; i++) {
		// test if the "tree" is inside the "territory"
		// if not, no "power" will be gained
		int intCount = 0;
		ivec2 ro = P[i];
		if (!(ro.x >= RMax.x || ro.x <= RMin.x || ro.y >= RMax.y || ro.y <= RMin.y)) {
			intCount = intersectTree(&R, ro);
		}

		// if not, calculate (2x) the gained "power"
		// gained area is the polygon defined by point ro and part of the convex hull
		if (!(intCount & 1)) {
			// get an opposite pair of points (one is on the added part and one not)
			int d0 = 0, d1;
			bool b0 = det(C[0] - ro, C[1] - ro) < 0, b1, b;  // det will work since the shape is convex and no three points are collinear
			if (b0) {
				// find a point that is not on the added part
				// since more than half of points are not on the added part, testing random numbers is efficient enough
				do {
					d1 = rand() % Cn;
					b1 = det(C[d1] - ro, C[(d1 + 1) % N] - ro) < 0;
				} while (b1 == b0);
			}
			else {
				// find a point on the added part
				ivec2 rou = ivec2(ro.x, clamp(ro.y, RMin.y, RMax.y));
				if (rou.y == C[tId].y) d1 = tId;  // if on top, return topmost point
				else if (rou.y == C[bId].y) d1 = bId;  // if on buttom, return buttommost point
				else d1 = getClosestIdY(&R, rou);  // if neither, search tree
				// if the point is not valid, check adjacent points
				if ((b1 = det(C[d1] - ro, C[(d1 + 1) % N] - ro) < 0) == b0) d1 = (d1 + Cn - 1) % Cn;
				if ((b1 = det(C[d1] - ro, C[(d1 + 1) % N] - ro) < 0) == b0) d1 = (d1 + 2) % Cn;
			}
			if (b0) swap(d0, d1);

#ifdef __DEBUG
			// debug code, check if the two points found are opposite
			b0 = det(C[d0] - ro, C[(d0 + 1) % N] - ro) < 0;
			b1 = det(C[d1] - ro, C[(d1 + 1) % N] - ro) < 0;
			if (b1 == b0) {
				throw(__LINE__);
			}
#endif

			// binary search to find the two turning points
			int s0, s1;
			int t0 = d0, t1 = d1 > d0 ? d1 : d1 + Cn, t;
			while (t1 - t0 > 1) {
				t = (t0 + t1) / 2;
				b = det(C[t % Cn] - ro, C[(t + 1) % Cn] - ro) < 0;
				if (b) t1 = t;
				else t0 = t;
			}
			s0 = t1 % Cn;
			t0 = d1, t1 = d0 < d1 ? d0 + Cn : d0, t;
			while (t1 - t0 > 1) {
				t = (t0 + t1) / 2;
				b = det(C[t % Cn] - ro, C[(t + 1) % Cn] - ro) < 0;
				if (b) t0 = t;
				else t1 = t;
			}
			s1 = t1 % Cn;

			// calculate the area using prefix sum array
			lint A = det(C[s0], ro) + det(ro, C[s1]);
			lint A1 = s1 ? Ps[s1 - 1] : 0;
			lint A0 = s0 ? Ps[s0 - 1] : 0;
			if (s1 < s0) A1 += totA;
			A -= A1 - A0;
			if (A > maxA) maxA = A;
		}
	}

	totA += maxA;
	printf("%lld", totA / 2);
	if (totA & 1) printf(".5\n");
	else printf(".0\n");

	return 0;
}