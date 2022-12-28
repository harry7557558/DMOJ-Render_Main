#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;


double vecnorm2(int n, const double* r) {
	double s = 0.0;
	for (int i = 0; i < n; i++) s += r[i] * r[i];
	return s;
}
double vecdot(int n, const double* u, const double* v) {
	double s = 0.0;
	for (int i = 0; i < n; i++) s += u[i] * v[i];
	return s;
}

// conjugate gradient from symmetric linear operator
// x must be initialized
int conjugateGradient(
	int n,
	std::function<void(const double* src, double* res)> A,
	const double* b, double* x, int maxiter, double tol
) {
	tol = tol * tol;
	// r = b - Ax
	double* r = new double[n];
	A(x, r);
	for (int i = 0; i < n; i++) r[i] = b[i] - r[i];
	double r20 = vecnorm2(n, r);
	// p = r
	double* p = new double[n];
	std::memcpy(p, r, n * sizeof(double));
	// loop
	double* Ap = new double[n];
	int k; for (k = 0; k < maxiter; k++) {
		// α = rᵀr / pᵀAp
		A(p, Ap);
		double alpha = r20 / vecdot(n, p, Ap);
		// x = x + αp
		for (int i = 0; i < n; i++) x[i] += alpha * p[i];
		// r = r - αAp
		for (int i = 0; i < n; i++) r[i] -= alpha * Ap[i];
		// β = r₁ᵀr₁ / r₀ᵀr₀
		double r21 = vecnorm2(n, r);
		if (r21 < tol) { k++; break; }
		double beta = r21 / r20;
		r20 = r21;
		// p = r + βp
		for (int i = 0; i < n; i++) p[i] = r[i] + beta * p[i];
	}
	delete[] r; delete[] p; delete[] Ap;
	return k;
}


// https://dmoj.ca/contest/rte1
std::vector<double> FindVoltages(
	int N,
	std::vector<int> u, std::vector<int> v, std::vector<double> w,
	std::vector<double> b)
{
	int M = (int)u.size();
	for (int i = 0; i < M; i++)
		u[i]--, v[i]--;

	std::vector<double> X; X.resize(N);

	// diagonal preconditioner
	double* diags = new double[N],
		* sqrtDiags = new double[N],
		* invSqrtDiags = new double[N];
	for (int i = 0; i < N; i++)
		diags[i] = 0.0;
	for (int i = 0; i < M; i++) {
		diags[u[i]] += w[i];
		diags[v[i]] += w[i];
	}
	for (int i = 0; i < N; i++)
		invSqrtDiags[i] = 1.0 / (sqrtDiags[i] = sqrt(diags[i]));

	// apply to b
	for (int i = 0; i < N; i++)
		b[i] *= invSqrtDiags[i];

	// CG
	double* src1 = new double[N];
	auto precondA = [&](const double* src, double* res) {
		for (int i = 0; i < N; i++)
			src1[i] = src[i] * invSqrtDiags[i],
			res[i] = src[i] * sqrtDiags[i];
		for (int i = 0; i < M; i++) {
			res[u[i]] -= src1[v[i]] * w[i];
			res[v[i]] -= src1[u[i]] * w[i];
		}
		for (int i = 0; i < N; i++)
			res[i] *= invSqrtDiags[i];
	};
	int niters = conjugateGradient(N, precondA, &b[0], &X[0], 1000000, 1e-10);

	// apply back to X
	for (int i = 0; i < N; i++)
		X[i] *= invSqrtDiags[i];

	fprintf(stderr, "%d iterations.\n", niters);

	return X;
}


int main() {
#ifndef ONLINE_JUDGE
	freopen("stdin.dat", "r", stdin);
#endif
	ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

	int N, M; double V0; cin >> N >> M >> V0;
	std::vector<int> U, V; std::vector<double> W;
	for (int i = 0; i < M; i++) {
		int u, v; double w;
		cin >> u >> v >> w;
		U.push_back(u), V.push_back(v), W.push_back(1.0 / w);
	}
	std::vector<double> B(N, 0.0);
	B[0] = 1.0; B[N - 1] = -1.0;

	std::vector<double> X = FindVoltages(N, U, V, W, B);

	double dV = X[0] - X[N - 1];
	for (int i = 0; i < N; i++) X[i] = V0 * X[i] / dV;
	
	for (int i = 0; i < M; i++) {
		double I = (X[U[i] - 1] - X[V[i] - 1]) * W[i];
		printf("%.12lf\n", I);
	}

	return 0;
}
