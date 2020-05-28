// Fit a path consist of splines into a trigonometric series
// Make sure to include "geometry.h" before including this file


// numerical integration
template<typename Fun> double NIntegral(Fun f, double a, double b, int n) {
	n *= 2;
	double u = (b - a) / n;
	double s = 0;
	for (int i = 1; i < n; i += 2) s += f(a + u * i);
	s *= 2;
	for (int i = 2; i < n; i += 2) s += f(a + u * i);
	s = 2 * s + f(a) + f(b);
	return s * u / 3;
}



// continuous Fourier series, 0 < t < 2π
void fitFourierSeries(const std::vector<spline3> &sp, int N, vec2 *A, vec2 *B) {
	const int L = sp.size();
	A[0] = B[0] = vec2(0.0);
	for (int i = 0; i < L; i++) {
		A[0] += (1. / 4)*sp[i].c3 + (1. / 3)*sp[i].c2 + (1. / 2)*sp[i].c1 + sp[i].c0;  // Integral[c(t),t,0,1]
	}
	A[0] /= L;
	for (int k = 1; k < N; k++) {
		A[k] = B[k] = vec2(0.0);
		double f = 2.*PI*k / L;
		for (int i = 0; i < L; i++) {
			double a = f, b = f * i;
			vec2 p0 = sp[i].c0, p1 = sp[i].c1, p2 = sp[i].c2, p3 = sp[i].c3;
			double a2 = a * a, a3 = a2 * a, a4 = a3 * a, sapb = sin(a + b), capb = cos(a + b), sb = sin(b), cb = cos(b);
			A[k] += (((a3 - 6 * a)*sapb + (3 * a2 - 6)*capb + 6 * cb)*p3 + ((a3 - 2 * a)*sapb + 2 * a2*capb + 2 * a*sb)*p2 + (a3*sapb + a2 * capb - a2 * cb)*p1 + a3 * (sapb - sb)*p0) / a4;  // Integral[sp(t)cos(at+b),t,0,1]
			B[k] += (((6 * a - a3)*capb + (3 * a2 - 6)*sapb + 6 * sb)*p3 + ((2 * a - a3)*capb + 2 * a2*sapb - 2 * a*cb)*p2 + (a2*sapb - a3 * capb - a2 * sb)*p1 - a3 * (capb - cb)*p0) / a4;  // Integral[sp(t)sin(at+b),t,0,1]
		}
		A[k] /= .5*L, B[k] /= .5*L;
	}

#if 1
	// throw the visualizing work to graphing calculators
	printf("(%lf", A[0].x);
	for (int k = 1; k < N; k++) {
		printf("%+lf*cos(%dt)%+lf*sin(%dt)", A[k].x, k, B[k].x, k);
	}
	printf(",%lf", A[0].y);
	for (int k = 1; k < N; k++) {
		double f = k;
		printf("%+lf*cos(%dt)%+lf*sin(%dt)", A[k].y, k, B[k].y, k);
	}
	printf(")\n");
#endif
}


// To-do:
// Add polar-angle-based Fourier series
// Use a ray intersector to evaluate r(θ)

