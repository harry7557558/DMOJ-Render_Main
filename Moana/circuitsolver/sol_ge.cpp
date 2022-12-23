#pragma GCC optimize "Ofast"
//#pragma GCC optimize "O0"
#include <bits/stdc++.h>
using namespace std;



// data
struct vec2 { int x; double y; };
#define MAXD 32768
int N, M, D; double V;
int W[MAXD][2]; std::vector<vec2> C[MAXD];
double R[MAXD], invR[MAXD];
double A[4096][4096];
double X[MAXD];


int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

    // input data
    cin >> N >> M >> V;
    for (int i = 0; i < M; i++) {
        cin >> W[i][0] >> W[i][1] >> R[i];
        W[i][0]--, W[i][1]--;
        invR[i] = 1.0 / R[i];
        C[W[i][0]].push_back(vec2{ i, -invR[i] });
        C[W[i][1]].push_back(vec2{ i, invR[i] });
    }

    // matrix
    for (int i = 0; i < N; i++) {
        for (vec2 k : C[i]) {
            A[i][W[k.x][0]] -= k.y;
            A[i][W[k.x][1]] += k.y;
        }
    }
    A[0][0] += 1.0;

    // X, dummy current from the power source => voltage at each node
    X[0] = 1.0;
    X[N - 1] = -1.0;

#if 0

    // Gaussian elimination
    for (int i = 0; i < N; i++) {
    	for (int j = 0; j < N; j++) if (j != i) {
    	    double m = A[j][i] / A[i][i];
    	    for (int k = i; k < N; k++)
    	        A[j][k] -= m * A[i][k];
    	    X[j] -= m * X[i];
    	}
    }
    for (int i = 0; i < N; i++)
        X[i] /= A[i][i];

#elif 0

    // LU
    for (int i = 0; i < N; i++) {
    	for (int j = i + 1; j < N; j++) {
    	    double m = A[j][i] / A[i][i];
    	    for (int k = i; k < N; k++)
    	        A[j][k] -= m * A[i][k];
    	    X[j] -= m * X[i];
    	}
    }
    for (int i = N - 1; i >= 0; i--) {
        double s = 0.0;
        for (int j = i + 1; j < N; j++)
            s += A[i][j] * X[j];
        X[i] = (X[i] - s) / A[i][i];
    }

#else

    // Cholesky
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            double s = 0.0;
            for (int k = 0; k < j; k++)
                s += A[i][k] * A[j][k];
            A[i][j] = (i == j) ? sqrt(A[i][i] - s)
                : (A[j][i] - s) / A[j][j];
        }
    }
    for (int i = 0; i < N; i++) {
        double s = 0.0;
        for (int j = 0; j < i; j++)
            s += A[i][j] * X[j];
        X[i] = (X[i] - s) / A[i][i];
    }
    for (int i = N - 1; i >= 0; i--) {
        double s = 0.0;
        for (int j = i + 1; j < N; j++)
            s += A[j][i] * X[j];
        X[i] = (X[i] - s) / A[i][i];
    }

#endif

    // get the solution
    double I_overall = V / (X[N - 1] - X[0]);
    for (int i = 0; i < M; i++) {
        double ans = I_overall * invR[i] * (-X[W[i][0]] + X[W[i][1]]);
        printf("%.12lf\n", ans);
    }

    return 0;
}
