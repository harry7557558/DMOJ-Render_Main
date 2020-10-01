// National Olympiad in Informatics, China, 2005, Problem 6
// Lemon Tree Under the Moon (月下柠檬树)

#include <bits/stdc++.h>
using namespace std;

double clamp(double x, double a, double b) {
	return x<a ? a : x>b ? b : x;
}

int N;  // number of circles
double alpha, cota;  // projection angle
double H[501];  // distance from the center of each layer's shadow to the trunk
double R[501];  // radius of each layer('s projection)

int SN = 0;
struct segment {
	double m, b;  // y=mx+b
	double x0, x1;  // starting and ending x-coordinates
} S[500];



int main() {
#ifdef __DEBUG
	freopen("stdin.dat", "r", stdin);
#endif
	// input
	cin >> N;
	cin >> alpha; cota = 1.0 / tan(alpha);
	for (int i = 0; i <= N; i++) {
		double h; cin >> h;
		H[i] = h + (i ? H[i - 1] : 0);
	}
	for (int i = 0; i < N; i++) {
		cin >> R[i];
	}
	R[N] = 0; N++;
	for (int i = 0; i < N; i++) H[i] *= cota;

	// calculate segments
	for (int i = 1; i < N; i++) {
		double d0 = H[i - 1], d1 = H[i];
		double r0 = R[i - 1], r1 = R[i];
		double sa = (r1 - r0) / (d1 - d0), ca = sqrt(1 - sa * sa);
		if (0.0*ca == 0.0) {  // segment exists
			double x0 = d0 - r0 * sa, y0 = r0 * ca;
			double x1 = d1 - r1 * sa, y1 = r1 * ca;
			if (!(x0 < x1)) throw("bug!");
			segment s;
			s.x0 = x0, s.x1 = x1;
			s.m = (y1 - y0) / (x1 - x0);
			s.b = y0 - s.m*x0;
			S[SN++] = s;
		}
	}

	// find the x-coordinates of all intersections
	vector<double> ints;
	// intersection between circles and x-axis
	for (int i = 0; i < N; i++) {
		if (R[i] == 0) ints.push_back(H[i]);
		else {
			ints.push_back(H[i] - R[i]);
			ints.push_back(H[i] + R[i]);
		}
	}
	// line endings
	for (int i = 0; i < SN; i++) {
		ints.push_back(S[i].x0);
		ints.push_back(S[i].x1);
	}
	// intersections between lines
	for (int i = 0; i < SN; i++) {
		segment s1 = S[i];
		for (int j = 0; j < i; j++) {
			segment s0 = S[j];
			double x = (s1.b - s0.b) / (s0.m - s1.m);
			if (x > s0.x0 && x<s0.x1 && x>s1.x0 && x < s1.x1)
				ints.push_back(x);
		}
	}
	// intersections between lines and circles
	for (int i = 0; i < SN; i++) {
		double m = S[i].m, b = S[i].b;
		for (int j = 0; j < N; j++) {
			double c = H[j], r = R[j];
			double A = m * m + 1, B = m * b - c, C = c * c + b * b - r * r;
			double delta = B * B - A * C;
			if (delta > 0.) {
				delta = sqrt(delta) / A, B /= -A;
				double x = B - delta;
				if (x - 1e-8 > S[i].x0 && x + 1e-8 < S[i].x1)
					ints.push_back(x);
				x = B + delta;
				if (x - 1e-8 > S[i].x0 && x + 1e-8 < S[i].x1)
					ints.push_back(x);
			}
		}
	}
	// intersections between circles
	for (int i = 0; i < N; i++) {
		double c1 = H[i], r1 = R[i];
		for (int j = 0; j < i; j++) {
			double c0 = H[j], r0 = R[j];
			double x = ((r0*r0 - r1 * r1) + (c1*c1 - c0 * c0)) / (2 * (c1 - c0));
			if (abs(x - c0) < r0 && abs(x - c1) < r1)
				ints.push_back(x);
		}
	}
	// sort the intersections
	std::sort(ints.begin(), ints.end());

	// sum the areas over all intervals
	double Area = 0;
	for (int i = 1; i < ints.size(); i++) {
		double x0 = ints[i - 1], x1 = ints[i];
		if (x1 - x0 < 1e-6) continue;
		// determine if the topmost in the interval is a segment or a circle
		double xc = 0.5*(x0 + x1);
		double circMax = 0; int circId = -1;
		for (int i = 0; i < N; i++) {
			double xd = xc - H[i];
			if (abs(xd) < R[i]) {
				double y = sqrt(R[i] * R[i] - xd * xd);
				if (y > circMax) circMax = y, circId = i;
			}
		}
		double segMax = 0; int segId = -1;
		for (int i = 0; i < SN; i++) {
			if (xc > S[i].x0 && xc < S[i].x1) {
				double y = S[i].m*xc + S[i].b;
				if (y > segMax) segMax = y, segId = i;
			}
		}
		// if it is a segment, the integral is the area of a trapezoid
		if (segMax > circMax && segId != -1) {
			segment s = S[segId];
			double y0 = s.m*x0 + s.b;
			double y1 = s.m*x1 + s.b;
			double dA = (y0 + y1)*(x1 - x0);
			Area += dA;
		}
		// if it is a circle...
		else if (circId != -1) {
			x0 = x0 - H[circId], x1 = x1 - H[circId];
			double r = R[circId], r2 = r * r;
			double A1 = r2 * asin(clamp(x1 / r, -1., 1.)) + x1 * sqrt(max(r2 - x1 * x1, 0.));
			double A0 = r2 * asin(clamp(x0 / r, -1., 1.)) + x0 * sqrt(max(r2 - x0 * x0, 0.));
			Area += A1 - A0;
		}
	}

	// print result
	printf("%.2lf\n", Area);

	return 0;
}
