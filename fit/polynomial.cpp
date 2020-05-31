// Fit a path consisting of splines to a single-variable polynomial

#include "Fit.h"




// C[0]+C[1]t+C[2]t²+...+C[N]tⁿ, 0<t<1
void fitPolynomial_Parametric(const std::vector<spline3> &sp, int N, vec2 *C, bool print = false, bool print_reverse = true) {
	const int Dif = 100;
	int T = sp.size(), M = Dif * T;
	double *x = new double[M], *y = new double[M], *t = new double[M];
	for (int i = 0; i < T; i++) {
		for (int u = 0; u < Dif; u++) {
			vec2 p = sp[i].eval(u / double(Dif));
			x[i*Dif + u] = p.x, y[i*Dif + u] = p.y;
			t[i*Dif + u] = (i*Dif + u) / double(M);  // or just for(i=0;i<M;i++)
		}
	}

	double *c = new double[N + 1];
	fitPolynomial(t, x, M, c, N);
	for (int i = 0; i < N + 1; i++) C[i].x = c[i];
	fitPolynomial(t, y, M, c, N);
	for (int i = 0; i < N + 1; i++) C[i].y = c[i];
	delete x; delete y; delete t; delete c;

	if (print) {
		// print_reverse: reflect at x-axis
		double _ = print_reverse ? -1.0 : 1.0;
		auto printF = [](double x) {
			int e = log10(abs(x));
			if (abs(e) > 4) {
				printf("%+.16lf*10^{%d}", x * pow(10, -e), e);
			}
			else printf("%+.16lf", x);
		};
		printf("(%.16lf", C[0].x);
		for (int i = 1; i <= N; i++) {
			printF(C[i].x);
			printf("*t^{%d}", i);
		}
		printf(",%.16lf", _*C[0].y);
		for (int i = 1; i <= N; i++) {
			printF(_*C[i].y);
			printf("*t^{%d}", i);
		}
		printf(")\n");
	}
}
// works bad




// similar to the trigonometric one
void fitPolynomial_angle(const std::vector<spline3> &sp, vec2 P, int N, double *C, bool print = false) {
	auto R = [&](double t)->double {
		int Count = 0;
		double r = splineIntersect(sp, P, vec2(sin(t), cos(t)), &Count);
		if (Count != 1) fprintf(stderr, "[%s::%d] Error: %d intersection(s) found with t=%.16lg.\n", __FILE__, __LINE__, Count, t);
		return r;
	};

	const int M = 2000;
	double *r = new double[M], *t = new double[M];
	for (int i = 0; i < M; i++) {
		t[i] = i * (2.*PI) / M;
		//t[i] = i * (.2*PI) / M;  // treat DMOJ logo
		r[i] = R(t[i]);
	}
	fitPolynomial(t, r, M, C, N);

	if (print) {
		// the coefficients can be extremly large or small
		printf("r(t)=%.16lf", C[0]);
		for (int i = 1; i <= N; i++) {
			if (abs(log10(abs(C[i]))) > 4) {
				int e = log10(abs(C[i]));
				printf("%+.16lf*10^{%d}*t^{%d}", C[i] * pow(10, -e), e, i);
			}
			else printf("%+.16lf*t^{%d}", C[i], i);
		}
		printf("\n");
	}
}





// DEBUG
// least square fitting, almost a copy-paste of the one in Fit.h
void FourierSeriesToPolynomial(int n, vec2* c, int N, double *C, double t0, double t1) {
	int L = N + 1;

	// Indefinite integral of xⁿsin(kx)
	auto Int = [](int n, int k) {
		/* Mathing...... */
	};

	double *sumxn = new double[2 * N + 1], *sumxny = new double[L];
	double e0 = t1 - t0, e = 1;
	for (int i = 0; i <= 2 * N; i++) {
		e *= e0;
		sumxn[i] = e / (i + 1);
	}
	for (int i = 0; i < L; i++) sumxny[i] = 0;
	for (int i = 0, j; i < n; i++) {
		double t;
		//for (j = 0, t = 1.0; j <= 2 * N; j++) sumxn[j] += t, t *= x[i];
		//for (j = 0, t = 1.0; j <= N; j++) sumxny[j] += t * y[i], t *= x[i];
	}

	// setup linear equations
	double *Mat = new double[L*L];
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			Mat[i*L + j] = sumxn[2 * N - i - j];
		}
	}
	double *X = new double[L];
	for (int i = 0; i < L; i++) X[i] = sumxny[N - i];
	delete sumxn; delete sumxny;

	// solve the linear system
	solveLinear(Mat, X, L);

	// output
	//for (int i = 0; i < L; i++) c[i] = X[N - i];
	delete X; delete Mat;
}

