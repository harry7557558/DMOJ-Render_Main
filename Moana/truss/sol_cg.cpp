#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>
using namespace std;



// sparse matrix

class LilMatrix {
    int n;  // n*n square matrix
    std::vector<std::map<int, double>> mat;  // list of dict?!

public:
    friend class CsrMatrix;

    LilMatrix(int n) {
        this->n = n;
        mat.resize(n);
    }
    int getN() const { return n; }
    void addValue(int row, int col, double val) {
        mat[row][col] += val;
    }
    void setValue(int row, int col, double val) {
        mat[row][col] = val;
    }
    double at(int i, int j, double default_ = 0.0) const {
        auto p = mat[i].find(j);
        if (p == mat[i].end()) return default_;
        return p->second;
    }

    // return true iff the matrix is strictly symmetric
    bool isSymmetric() const {
        for (int i = 0; i < n; i++) {
            for (std::pair<int, double> jw : mat[i]) {
                int j = jw.first;
                if (j >= i) break;
                auto p = mat[j].find(i);
                if (p == mat[j].end() || p->second != jw.second)
                    return false;
            }
        }
        return true;
    }

    // visualize
    void print(FILE* fp = stdout) const {
        char* c = new char[n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) c[j] = ' ';
            for (std::pair<int, double> jw : mat[i]) {
                if (jw.second > 0.0) c[jw.first] = '+';
                if (jw.second < 0.0) c[jw.first] = '-';
                if (isnan(jw.second)) c[jw.first] = '#';
                //printf("%lf ", jw.second);
            }
            fwrite(c, 1, n, fp);
            fputc('\n', fp);
        }
        delete c;
    }
    void printOctave(FILE* fp = stdout) const {
        fprintf(fp, "[");
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                fprintf(fp, "%.12lg%c", this->at(i, j), j + 1 == n ? ';' : ',');
            }
        fprintf(fp, "];");
    }

    // res = mat * src
    template<typename vec>
    void matvecmul(const vec* src, vec* res) const {
        for (int i = 0; i < n; i++) {
            res[i] = vec(0.0);
            for (std::pair<int, double> jw : mat[i]) {
                res[i] += jw.second * src[jw.first];
            }
        }
    }

    // transpose
    LilMatrix transpose() const {
        LilMatrix res(n);
        for (int i = 0; i < n; i++) {
            for (std::pair<int, double> jw : mat[i]) {
                res.addValue(jw.first, i, jw.second);
            }
        }
        return res;
    }

    // factorization, may change the current matrix
    // result in a upper triangular matrix
    LilMatrix incompleteCholesky1(double default_diag) {
        LilMatrix res(n);
        for (int i = 0; i < n; i++) {
            double diag = sqrt(abs(this->at(i, i, default_diag)));
            res.addValue(i, i, diag);
            for (std::pair<int, double> kw : mat[i]) {
                int k = kw.first;
                if (k > i) res.addValue(i, k, kw.second / diag);
            }
        }
        return res;
    }

};

class CsrMatrix {
    int n;  // n*n square matrix
    std::vector<int> rows;  // see Wikipedia
    std::vector<int> cols;
    std::vector<double> vals;

public:

    CsrMatrix(): n(0) {}
    CsrMatrix(const LilMatrix& lil) {
        this->n = lil.n;
        rows.push_back(0);
        for (int i = 0; i < n; i++) {
            for (std::pair<int, double> indice : lil.mat[i]) {
                if (indice.second != 0.0) {
                    cols.push_back(indice.first);
                    vals.push_back(indice.second);
                }
            }
            rows.push_back((int)cols.size());
        }
    }
    int getN() const { return n; }

    // square of norm of a vector
    double vecnorm2(const double* r) const {
        double ans = 0.0;
        for (int i = 0; i < n; i++) ans += r[i] * r[i];
        return ans;
    }

    // dot product between two vectors
    double vecdot(const double* u, const double* v) const {
        double ans = 0.0;
        for (int i = 0; i < n; i++) ans += u[i] * v[i];
        return ans;
    }

    // multiply by scalar
    void operator*=(double k) {
        for (int i = 0, l = rows.back(); i < l; i++)
            vals[i] *= k;
    }

    // res = mat * src
    void matvecmul(const double* src, double* res) const {
        for (int i = 0; i < n; i++) {
            res[i] = 0.0;
            for (int ji = rows[i]; ji < rows[i + 1]; ji++) {
                res[i] += vals[ji] * src[cols[ji]];
            }
        }
    }

    // res = mat^T * src
    void vecmatmul(const double* src, double* res) const {
        for (int j = 0; j < n; j++)
            res[j] = 0.0;
        for (int i = 0; i < n; i++) {
            for (int ji = rows[i]; ji < rows[i + 1]; ji++) {
                res[cols[ji]] += vals[ji] * src[i];
            }
        }
    }

    // return u^T * mat * v
    double vecmatvecmul(const double* u, const double* v) const {
        double res = 0.0;
        for (int i = 0; i < n; i++) {
            for (int ji = rows[i]; ji < rows[i + 1]; ji++) {
                res += u[i] * vals[ji] * v[cols[ji]];
            }
        }
        return res;
    }

    // solve a lower triangular matrix equation
    void lowerSolve(double* x) const {
        for (int i = 0; i < n; i++) {
            double s = 0.0;
            for (int ji = rows[i]; ji < rows[i + 1] - 1; ji++)
                s += vals[ji] * x[cols[ji]];
            int ji = rows[i + 1] - 1;
            assert(i == cols[ji]);
            x[i] = (x[i] - s) / vals[ji];
        }
    }
    // solve an upper triangular matrix equation
    void upperSolve(double* x) const {
        for (int i = n - 1; i >= 0; i--) {
            double s = 0.0;
            for (int ji = rows[i + 1] - 1; ji > rows[i]; ji--)
                s += vals[ji] * x[cols[ji]];
            int ji = rows[i];
            assert(i == cols[ji]);
            x[i] = (x[i] - s) / vals[ji];
        }
    }

    // evaluate (mat*x-b)^2
    double linequError2(const double* x, const double* b) const {
        double toterr = 0.0;
        for (int i = 0; i < n; i++) {
            double err = -b[i];
            for (int ji = rows[i]; ji < rows[i + 1]; ji++) {
                err += vals[ji] * x[cols[ji]];
            }
            toterr += err * err;
        }
        return toterr;
    }
};

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
    tol = double(n) * tol * tol;
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



// data
int N, S, M; double EA;
double J[15000];
double Fa[15000], Fm[20000], U[15000];
bool isFixed[5000]; int Ns[5000];
int C[20000][2]; double dJ[20000][3], K[20000];
double Fas[15000], Us[15000];


int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

#ifndef ONLINE_JUDGE
    freopen("data/12.in", "r", stdin);
#endif

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
    LilMatrix lil(D);
    for (int i = 0; i < M; i++) {
        int a = C[i][0], b = C[i][1];
        int as = Ns[a], bs = Ns[b];
        // dp (normalize)
        dJ[i][0] = J[3 * b] - J[3 * a];
        dJ[i][1] = J[3 * b + 1] - J[3 * a + 1];
        dJ[i][2] = J[3 * b + 2] - J[3 * a + 2];
        K[i] = 1.0 / sqrt(dJ[i][0] * dJ[i][0] + dJ[i][1] * dJ[i][1] + dJ[i][2] * dJ[i][2]);
        dJ[i][0] *= K[i], dJ[i][1] *= K[i], dJ[i][2] *= K[i];
        //K[i] *= EA;
        // add to matrix
        for (int u = 0; u < 3; u++) for (int v = 0; v < 3; v++) {
            double d = K[i] * dJ[i][u] * dJ[i][v];
            if (!isFixed[a]) lil.addValue(3 * as + u, 3 * as + v, d);
            if (!isFixed[a] && !isFixed[b]) {
                lil.addValue(3 * as + u, 3 * bs + v, -d);
                lil.addValue(3 * bs + u, 3 * as + v, -d);
            }
            if (!isFixed[b]) lil.addValue(3 * bs + u, 3 * bs + v, d);
        }
    }

    // setup vector
    for (int i = 0; i < N; i++) {
        if (isFixed[i]) continue;
        Fas[3 * Ns[i] + 2] = Fa[3 * i + 2];
    }

    // solve the linear system (Cholesky)
    CsrMatrix csr(lil);
    LilMatrix iclil = lil.incompleteCholesky1(1.0);
    CsrMatrix precondU(iclil), precondL(iclil.transpose());
    precondL.lowerSolve(Fas);
    double* src1 = new double[D];
    auto precondA = [&](const double* src, double* res) {
        memcpy(src1, src, sizeof(double) * D);
        precondU.upperSolve(src1);
        csr.matvecmul(src1, res);
        precondL.lowerSolve(res);
    };
    int niters = conjugateGradient(D, precondA, Fas, Us, 100000, 1e-4);
    precondU.upperSolve(Us);
    delete[] src1;

    fprintf(stderr, "%d iterations\n", niters);

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

#ifdef ONLINE_JUDGE
    for (int i = 0; i < M; i++)
        printf("%lf%c", Fm[i], i + 1 == M ? '\n': ' ');
#endif

    return 0;
}
