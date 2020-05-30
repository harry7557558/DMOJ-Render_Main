// Fitting template
// Make sure to include "svg.cpp" before including this header


#ifndef _INC_FIT_H

#define _INC_FIT_H




// Fit point set to polynomial, c[k] has degree k
void fitPolynomial(double *x, double *y, int n, double *c, int N) {
	int L = N + 1;

	// calculate sums and powers
	// the sums can be replaced by integral
	double *sumxn = new double[2 * N + 1], *sumxny = new double[L];
	for (int i = 0; i <= 2 * N; i++) sumxn[i] = 0;
	for (int i = 0; i < L; i++) sumxny[i] = 0;
	for (int i = 0, j; i < n; i++) {
		double t;
		for (j = 0, t = 1.0; j <= 2 * N; j++) sumxn[j] += t, t *= x[i];
		for (j = 0, t = 1.0; j <= N; j++) sumxny[j] += t * y[i], t *= x[i];
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
	delete sumxn, sumxny;

	// solve the linear system
	solveLinear(Mat, X, L);

	// output
	for (int i = 0; i < L; i++) c[i] = X[N - i];
	delete X; delete Mat;
}


// polynomial evaluation
double evalPolynomial(const double *c, int n, double t) {
	double r = c[n];
	for (int i = n; i > 0; i--) {
		r = r * t + c[i - 1];
	}
	return r;
}



#endif  // _INC_FIT_H
