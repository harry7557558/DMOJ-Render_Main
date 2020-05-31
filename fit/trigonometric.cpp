// Fit a path consisting of splines to trigonometric series

#include "Fit.h"




// continuous Fourier series, 0 < t < 2π
void fitFourierSeries(const std::vector<spline3> &sp, int N, vec2 *A, vec2 *B, bool print = false, bool print_reverse = true) {
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

	if (print) {
		// if print_reverse is true, this output will be reflected at x-axis to fit graphing calculators
		double _ = print_reverse ? -1.0 : 1.0;
		printf("(%lf", A[0].x);
		for (int k = 1; k < N; k++) {
			printf("%+lf*cos(%d*t)%+lf*sin(%d*t)", A[k].x, k, B[k].x, k);
		}
		printf(",%lf", _*A[0].y);
		for (int k = 1; k < N; k++) {
			double f = k;
			printf("%+lf*cos(%d*t)%+lf*sin(%d*t)", _*A[k].y, k, _*B[k].y, k);
		}
		printf(")\n");
	}
}






// [slow] angle-based Fourier series - let P be the origin, fit r(θ)
#include <chrono>
void fitFourierSeries_angle(const std::vector<spline3> &sp, vec2 P, int N, vec2 *C, bool print = false) {
	auto t0 = std::chrono::high_resolution_clock::now();

	auto R = [&](double t)->double {
		int Count = 0;
		double r = splineIntersect(sp, P, vec2(sin(t), cos(t)), &Count);
		if (Count != 1) fprintf(stderr, "[%s::%d] Error: %d intersection(s) found with t=%.16lg.\n", __FILE__, __LINE__, Count, t);
		return r;
	};

	// test shows accuracy mostly depend on min_dif and performance mostly depend on dif
	const int dif = 12;
	const int min_dif = 200;
	//const int dif = 200;
	//const int min_dif = 10000;

	C[0] = vec2((.5 / PI) * NIntegral(R, 0, 2.*PI, min_dif), 0.0);
	for (int k = 1; k < N; k++) {
		C[k] = (1. / PI) * NIntegral([&](double t)->vec2 {
			return R(t) * vec2(cos(k*t), sin(k*t));
		}, 0, 2.*PI, max(k*dif, min_dif));
	}

	if (print) {
		printf("Time Elapsed: %lfms\n", 1000.*std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count());
		printf("r(t)=%.4lg", C[0].x);
		for (int k = 1; k < N; k++) {
			printf("%+.8lf*cos(%d*t)%+.8lf*sin(%d*t)", C[k].x, k, C[k].y, k);
			//if (k % 5 == 0) printf("%+.4lg*cos(%d*t)", C[k].x, k);  // to treat DMOJ logo
		}
		printf("\n");
	}
}



