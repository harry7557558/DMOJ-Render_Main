// Data of DMOJ logo and related operations


#ifndef _INC_DMOJ_LOGO

#define _INC_DMOJ_LOGO


#include "geometry.h"


// from https://dmoj.algome.me/texoid/bcbb015731331ea2276676e75931bfd32868d15b/svg
#include <string>
const std::string DMOJ_LOGO_OUTER_CIRCLE = "M218.335 253.597C156.757 254.593 104.511 202.402 104.941 140.289C105.374 77.582 156.339 26.593 219.203 27.468C281.273 28.328 331.71 76.648 331.265 142.41C330.855 203.277 280.902 253.75 218.343 253.597Z";
const std::string DMOJ_LOGO_INNER_CIRCLE = "M216.578 35.781C162.855 35.906 109.035 81.753 113.609 149.113C116.906 197.597 160.359 249.867 227.12 244.992C277.667 241.3 323.98 197.441 322.972 138.562C321.995 81.476 274.285 34.625 216.578 35.781";
const std::string DMOJ_LOGO_ASTERISK = "M235.191 145.925C242.382 155.992 250.8 164.75 258.773 173.847C267.835 184.191 267.956 184.023 256.859 191.804C253.292 194.304 249.804 196.929 246.456 199.711C243.331 202.3 241.222 202.886 238.953 198.484C232.581 186.121 217.449 160.015 216.027 160.269C214.667 160.519 199.867 186.718 192.933 198.812C190.656 202.781 188.925 202.257 185.902 199.785C180.519 195.378 175.027 191.031 169.167 187.316C165.429 184.949 165.39 183.289 168.136 180.242C177.218 170.148 196.585 147.636 197.738 146.023C183.39 143.046 169.648 140.109 155.859 137.378C150.695 136.355 148.992 135.187 151.277 129.019C153.429 123.218 156.433 117.375 157.035 111.078C157.566 105.503 160.23 105.402 164.245 107.211C176.597 112.769 202.921 124.464 204.41 124.933C204.945 114.41 200.808 85.148 200.113 80.609C199.601 77.281 200.699 75.914 204.343 76.05C211.636 76.32 218.98 76.586 226.234 75.984C231.843 75.519 232.734 78.058 231.921 82.429C229.347 96.312 229.171 110.398 228.058 125.175C240.945 119.535 253.538 114.328 265.843 108.511C272.816 105.218 273.992 105.371 275.718 112.476C277.16 118.406 279.468 124.117 281.429 129.851C283.476 135.832 280.687 136.464 276.538 137.324C262.913 140.144 249.3 142.996 235.187 145.933";

// this one is for debug
const std::string SVG_DEBUG_CASE = "M283.659,80.2481 C295.706,103.634 306.346,148.288 306.346,156.789 C306.346,165.301 296.57,173.319 304.927,185.849 C313.428,198.606 330.441,216.328 330.441,229.085 C330.441,241.842 299.253,232.631 309.183,254.599 C315.556,269.483 303.509,268.774 303.509,268.774 C317.684,279.413 294.298,285.787 299.962,300.671 C305.636,315.556 301.381,332.569 269.483,332.569 C209.955,332.569 213.491,332.569 213.491,375.095 C213.491,417.621 61.1077,416.203 61.118,375.804 C61.1077,335.406 68.2006,304.218 68.2006,304.218 C68.2006,243.26 20,277.995 20,136.95 C20,58.2706 90.1679,20 141.195,20 C192.233,20 275.158,55.4334 279.413,24.2557 C294.298,24.2557 302.09,56.1427 283.659,80.2481";





#include <vector>

// an old code that splits a svg path into cubic arcs
bool splitPath(std::string S, std::vector<spline3> &V) {

#define isFloat(c) ((c >= '0' && c <= '9') || c == '-' || c == '.')
#define readFloat(r) { \
		while (d < n && (S[d] == ' ' || S[d] == ',')) d++; \
		if (d >= n || !isFloat(S[d])) return false; \
		unsigned sz; \
		(r) = std::stod(&S[d], &sz); \
		d += sz; }
#define readPoint(v) { readFloat((v).x); readFloat((v).y); }

	char cmd = '\0'; vec2 P(0, 0), P0(0, 0), P1(NAN, NAN);
	for (int d = 0, n = S.size(); d < n;) {
		while (d < n && (S[d] == ' ' || S[d] == ',')) d++;

		if (std::string("MZLHVCSQTA").find(S[d] >= 'a' ? S[d] - 32 : S[d]) != -1) cmd = S[d], d++;
		else if (!isFloat(S[d])) return false;

		switch (cmd) {
		case 'M':; case 'm': {
			vec2 Q; readPoint(Q);
			if (cmd == 'm') P = P0 + Q;
			else P = Q;
			P0 = P, P1 = vec2(NAN);
			break;
		}
		case 'Z':; case 'z': {
			if ((P - P0).sqr() > 1e-12) V.push_back(spline3_fromSegment(P, P0));
			P1 = vec2(NAN);
			break;
		}
		case 'L':; case 'l': {
			vec2 Q; readPoint(Q);
			if (cmd == 'l') Q = P + Q;
			if ((Q - P).sqr() > 1e-12) V.push_back(spline3_fromSegment(P, Q));
			P1 = P, P = Q;
			break;
		}
		case 'H':; case 'h': {
			double c; readFloat(c);
			vec2 Q = P;
			if (cmd == 'H') Q.x = c;
			else Q.x += c;
			V.push_back(spline3_fromSegment(P, Q));
			P1 = P, P = Q;
			break;
		}
		case 'V':; case 'v': {
			double c; readFloat(c);
			vec2 Q = P;
			if (cmd == 'V') Q.y = c;
			else Q.y += c;
			V.push_back(spline3_fromSegment(P, Q));
			P1 = P, P = Q;
			break;
		}
		case 'C':; case 'c': {
			vec2 B, C, D;
			readPoint(B); readPoint(C); readPoint(D);
			if (cmd == 'c') B = B + P, C = C + P, D = D + P;
			V.push_back(spline3_fromBezier3(P, B, C, D));
			P1 = C, P = D;
			break;
		}
		case 'S':; case 's': {
			if (isnan(P1.x)) return 0;
			vec2 B = P * 2.0 - P1;
			vec2 C, D;
			readPoint(C); readPoint(D);
			if (cmd == 's') C = P + C, D = P + D;
			V.push_back(spline3_fromBezier3(P, B, C, D));
			P1 = C, P = D;
			break;
		}
		case 'Q':; case 'q': {
			vec2 B, C;
			readPoint(B); readPoint(C);
			if (cmd == 'q') B = B + P, C = C + P;
			V.push_back(spline3_fromBezier2(P, B, C));
			P1 = B, P = C;
			break;
		}
		case 'T':; case 't': {
			if (isnan(P1.x)) return 0;
			vec2 B = P * 2.0 - P1;
			vec2 C; readPoint(C);
			if (cmd == 't') C = P + C;
			V.push_back(spline3_fromBezier2(P, B, C));
			P1 = B, P = C;
			break;
		}
		case 'A':; case 'a': {
			return false;  // doable using least square fitting but the code can be pretty messy
		}
		default: {
			return false;
		}
		}
	}
	return true;
#undef isFloat
#undef readFloat
#undef readPoint
}

// an old code that converts a svg shape to a C++ string
#include <sstream>
#include <iomanip>
std::string toPath(const std::vector<spline3> &v) {
	auto toBezier = [](const spline3 &p, vec2 &A, vec2 &B, vec2 &C, vec2 &D) ->int {
		int N = p.c3.sqr() ? 3 : p.c2.sqr() ? 2 : p.c1.sqr() ? 1 : 0;
		if (N < 1) A = p.c0, B = A + p.c1;
		else if (N == 2) A = p.c0, B = p.c1*.5 + A, C = p.c2 - A + B * 2.0;
		else if (N == 3) A = p.c0, B = p.c1 / 3. + A, C = p.c2 / 3. + B * 2.0 - A, D = p.c3 + (C - B) * 3.0 + A;
		return N;
	};
	std::string s;
	if (v.empty()) return s;

	vec2 A, B, C, D, P = vec2(NAN);
	auto toString = [](vec2 p) ->std::string {
		std::stringstream ss;
		if (abs(p.x) < 1e-4) p.x = 0;
		if (abs(p.y) < 1e-4) p.y = 0;
		ss << std::setprecision(4) << p.x << "," << p.y;
		return std::string(ss.str());
	};
	for (int i = 0, l = v.size(); i < l; i++) {
		int n = toBezier(v[i], A, B, C, D);
		if (!((P - A).sqr() < 1e-4)) s += "M" + toString(A) + " ", P = A;
		s += n == 3 ? 'C' : n == 2 ? 'Q' : n == 1 ? 'L' : ' ';
		if (n >= 1) s += toString(B) + " ", P = B;
		if (n >= 2) s += toString(C) + " ", P = C;
		if (n >= 3) s += toString(D) + " ", P = D;
	}
	while (s[s.size() - 1] == ' ') s.erase(s.size() - 1, 1);
	return s;
}





// copy-pasted from an online integral calculator
// use Green's formula - note the conditions are important

double calcArea(const std::vector<spline3> &V) {
	double A = 0.0;
	for (int i = 0, n = V.size(); i < n; i++) {
		double x0 = V[i].c0.x, x1 = V[i].c1.x, y1 = V[i].c1.y, x2 = V[i].c2.x, y2 = V[i].c2.y, x3 = V[i].c3.x, y3 = V[i].c3.y;
		A += (30 * x3 + 36 * x2 + 45 * x1 + 60 * x0)*y3 + (24 * x3 + 30 * x2 + 40 * x1 + 60 * x0)*y2 + (15 * x3 + 20 * x2 + 30 * x1 + 60 * x0)*y1;
	}
	return A / 60.;
}

vec2 calcCOM(const std::vector<spline3> &V) {  // center of mass
	double A = 0.0; vec2 P = vec2(0.);
	for (int i = 0, n = V.size(); i < n; i++) {
		double x0 = V[i].c0.x, y0 = V[i].c0.y, x1 = V[i].c1.x, y1 = V[i].c1.y, x2 = V[i].c2.x, y2 = V[i].c2.y, x3 = V[i].c3.x, y3 = V[i].c3.y;
		A += (30 * x3 + 36 * x2 + 45 * x1 + 60 * x0)*y3 + (24 * x3 + 30 * x2 + 40 * x1 + 60 * x0)*y2 + (15 * x3 + 20 * x2 + 30 * x1 + 60 * x0)*y1;
		P += vec2(((140 * x3*x3 + (315 * x2 + 360 * x1 + 420 * x0)*x3 + 180 * x2*x2 + (420 * x1 + 504 * x0)*x2 + 252 * x1*x1 + 630 * x0*x1 + 420 * x0*x0)*y3 + (105 * x3*x3 + (240 * x2 + 280 * x1 + 336 * x0)*x3 + 140 * x2*x2 + (336 * x1 + 420 * x0)*x2 + 210 * x1*x1 + 560 * x0*x1 + 420 * x0*x0)*y2 + (60 * x3*x3 + (140 * x2 + 168 * x1 + 210 * x0)*x3 + 84 * x2*x2 + (210 * x1 + 280 * x0)*x2 + 140 * x1*x1 + 420 * x0*x1 + 420 * x0*x0)*y1),
			-((140 * x3 + 105 * x2 + 60 * x1)*y3*y3 + ((315 * x3 + 240 * x2 + 140 * x1)*y2 + (360 * x3 + 280 * x2 + 168 * x1)*y1 + (420 * x3 + 336 * x2 + 210 * x1)*y0)*y3 + (180 * x3 + 140 * x2 + 84 * x1)*y2*y2 + ((420 * x3 + 336 * x2 + 210 * x1)*y1 + (504 * x3 + 420 * x2 + 280 * x1)*y0)*y2 + (252 * x3 + 210 * x2 + 140 * x1)*y1*y1 + (630 * x3 + 560 * x2 + 420 * x1)*y0*y1 + (420 * x3 + 420 * x2 + 420 * x1)*y0*y0));
	}
	return P / (14.*A);
}



// bounding box

vec2 getFunctionRange(double c0, double c1, double c2, double c3) {  // cubic polynomial in [0,1]
	vec2 r(c0, c0 + c1 + c2 + c3);
	if (r.x > r.y) std::swap(r.x, r.y);

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

void getBoundingBox(const std::vector<spline3> &sp, vec2 &Min, vec2 &Max) {
	Min = vec2(INFINITY), Max = -Min;
	for (int i = 0, n = sp.size(); i < n; i++) {
		vec2 tMin, tMax;
		splineBoundingBox(sp[i], tMin, tMax);
		Min = pMin(Min, tMin), Max = pMax(Max, tMax);
	}
}



// operations

void translateShape(std::vector<spline3> &sp, vec2 d) {
	for (int i = 0, l = sp.size(); i < l; i++) {
		sp[i].c0 += d;
	}
}
void scaleShape(std::vector<spline3> &sp, double sc) {
	for (int i = 0, l = sp.size(); i < l; i++) {
		sp[i].c0 *= sc, sp[i].c1 *= sc, sp[i].c2 *= sc, sp[i].c3 *= sc;
	}
}
void mirrorShape_y(std::vector<spline3> &sp) {  // useful in converting screen and image coordinates
	for (int i = 0, l = sp.size(); i < l; i++) {
		sp[i].c0.y *= -1, sp[i].c1.y *= -1, sp[i].c2.y *= -1, sp[i].c3.y *= -1;
	}
}
// Implement rotation and reverse direction when needed



#endif  // _INC_DMOJ_LOGO
