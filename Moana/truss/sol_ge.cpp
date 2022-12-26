#pragma GCC optimize "Ofast"
//#pragma GCC optimize "O0"
#include <bits/stdc++.h>
using namespace std;


int N, S, M; double EA;
double J[15000];
double Fa[15000], Fm[20000], U[15000];
bool isFixed[5000]; int Ns[5000];
int C[20000][2]; double dJ[20000][3], K[20000];
double Gs[4096][4096]; double Us[15000];


int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

    // input data
    cin >> N >> S >> M >> EA;
    int D = 3 * (N - S);
    for (int i = 0; i < N; i++) {
        cin >> J[3 * i] >> J[3 * i + 1] >> J[3 * i + 2]
            >> Fa[3 * i + 2];
        Fa[3 * i + 2] *= -1.0;
    }
    for (int i = 0; i < S; i++) {
        int s; cin >> s; s--;
        isFixed[s] = true;
        Fa[3 * s + 2] = 0.0;
    }
    for (int i = 0; i < M; i++) cin >> C[i][0], C[i][0]--;
    for (int i = 0; i < M; i++) cin >> C[i][1], C[i][1]--;

    // setup matrix
    int ps = 0;
    for (int i = 0; i < N; i++)
        Ns[i] = ps, ps += !isFixed[i];
    for (int i = 0; i < M; i++) {
        int a = C[i][0], b = C[i][1];
        int as = Ns[a], bs = Ns[b];
        // dp (normalize)
        dJ[i][0] = J[3 * b] - J[3 * a];
        dJ[i][1] = J[3 * b + 1] - J[3 * a + 1];
        dJ[i][2] = J[3 * b + 2] - J[3 * a + 2];
        K[i] = 1.0 / sqrt(dJ[i][0] * dJ[i][0] + dJ[i][1] * dJ[i][1] + dJ[i][2] * dJ[i][2]);
        dJ[i][0] *= K[i], dJ[i][1] *= K[i], dJ[i][2] *= K[i];
        K[i] *= EA;
        // add to matrix
        for (int u = 0; u < 3; u++) for (int v = 0; v < 3; v++) {
            double d = K[i] * dJ[i][u] * dJ[i][v];
            if (!isFixed[a]) Gs[3 * as + u][3 * as + v] += d;
            if (!isFixed[a] && !isFixed[b]) {
                Gs[3 * as + u][3 * bs + v] -= d;
                Gs[3 * bs + u][3 * as + v] -= d;
            }
            if (!isFixed[b]) Gs[3 * bs + u][3 * bs + v] += d;
        }
    }

    // setup vector
    for (int i = 0; i < N; i++) {
        if (isFixed[i]) continue;
        Us[3 * Ns[i] + 2] = Fa[3 * i + 2];
    }

    // solve the linear system (Cholesky)
    for (int i = 0; i < D; i++) {
        for (int j = 0; j <= i; j++) {
            double s = 0.0;
            for (int k = 0; k < j; k++)
                s += Gs[i][k] * Gs[j][k];
            Gs[i][j] = (i == j) ? sqrt(Gs[i][i] - s)
                : (Gs[j][i] - s) / Gs[j][j];
        }
    }
    for (int i = 0; i < D; i++) {
        double s = 0.0;
        for (int j = 0; j < i; j++)
            s += Gs[i][j] * Us[j];
        Us[i] = (Us[i] - s) / Gs[i][i];
    }
    for (int i = D - 1; i >= 0; i--) {
        double s = 0.0;
        for (int j = i + 1; j < D; j++)
            s += Gs[j][i] * Us[j];
        Us[i] = (Us[i] - s) / Gs[i][i];
    }

    // for (int i = 0; i < D; i += 3)
    //     printf("%lf %lf %lf\n", Us[i], Us[i + 1], Us[i + 2]);

    // get member forces
    for (int i = 0; i < N; i++) if (!isFixed[i]) {
        int is = Ns[i];
        for (int u = 0; u < 3; u++)
            U[3 * i + u] = Us[3 * is + u];
    }
    for (int i = 0; i < M; i++) {
        int a = C[i][0], b = C[i][1];
        for (int u = 0; u < 3; u++) {
            Fm[i] -= dJ[i][u] * U[3 * a + u];
            Fm[i] += dJ[i][u] * U[3 * b + u];
        }
        Fm[i] *= K[i];
    }

    for (int i = 0; i < M; i++)
        printf("%lf%c", Fm[i], i + 1 == M ? '\n': ' ');

    return 0;
}
