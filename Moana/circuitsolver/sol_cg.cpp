#pragma GCC optimize "Ofast"
//#pragma GCC optimize "O0"
#include <bits/stdc++.h>
using namespace std;



// https://github.com/harry7557558/Graphics/blob/master/simulation/heatequ/mesh_smoothing_cpp/sparse.h

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
struct vec2 { int x; double y; };
#define MAXD 32768
int N, M, D; double V;
int W[MAXD][2]; std::vector<vec2> C[MAXD];
double R[MAXD], invR[MAXD], RiC[MAXD];
double X[MAXD], Y[MAXD];


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

    // get the node with most connections
    for (int i = 0; i < N; i++) RiC[i] = 0;
    RiC[0] = RiC[N - 1] = 1;
    for (int i = 0; i < M; i++)
        RiC[W[i][0]]++, RiC[W[i][1]]++;
    int mostConnecedNode = 0;
    for (int i = 1; i < N; i++)
        if (RiC[i] > RiC[mostConnecedNode])
            mostConnecedNode = i;

    // Y, dummy current from the power source
    Y[0] = 1.0;
    Y[N - 1] = -1.0;

    // X, initial guess, voltage at each node
    srand(0);
    for (int i = 0; i < N; i++)
        X[i] = -1.0 + 2.0 * rand() / (double)RAND_MAX;

    // linear operator to solve
    auto CTRiC = [&](const double* src, double* res) {
        for (int i = 0; i < M; i++)
            RiC[i] = invR[i] * (-src[W[i][0]] + src[W[i][1]]);
        for (int i = 0; i < N; i++)
            res[i] = 0.0;
        for (int i = 0; i < M; i++)
            res[W[i][0]] -= RiC[i], res[W[i][1]] += RiC[i];
        res[mostConnecedNode] += src[mostConnecedNode];
    };

    // represent the linear operator as a matrix
    LilMatrix lil(N);
    for (int i = 0; i < N; i++) {
        for (vec2 k : C[i]) {
            lil.addValue(i, W[k.x][0], -k.y);
            lil.addValue(i, W[k.x][1], k.y);
        }
    }
    lil.addValue(mostConnecedNode, mostConnecedNode, 1.0);
    CsrMatrix csr(lil);

    // Preconditioning:
    // AX = Y  =>  φ⁻¹Aφ⁻ᵀ φᵀX = φ⁻¹Y,
    // where φ⁻¹Aφ⁻ᵀ has a lower condition number.
#if 0
    // diagonal preconditioner
    double* invSqrtDiags = new double[N];
    for (int i = 0; i < N; i++)
        invSqrtDiags[i] = 1.0 / sqrt(lil.at(i, i, 1.0));
    for (int i = 0; i < N; i++)
        Y[i] *= invSqrtDiags[i];
    double* src1 = new double[N];
    auto precondA = [&](const double* src, double* res) {
        for (int i = 0; i < N; i++)
            src1[i] = src[i] * invSqrtDiags[i];
        CTRiC(src1, res);
        for (int i = 0; i < N; i++)
            res[i] *= invSqrtDiags[i];
    };
    int niters = conjugateGradient(N, precondA, Y, X, 100000, 1e-10);
    for (int i = 0; i < N; i++)
        X[i] *= invSqrtDiags[i];
    delete[] invSqrtDiags; delete[] src1;
#else
    // incomplete Cholesky decomposition
    LilMatrix iclil = lil.incompleteCholesky1(1.0);
    CsrMatrix precondU(iclil), precondL(iclil.transpose());
    precondL.lowerSolve(Y);
    double* src1 = new double[N];
    auto precondA = [&](const double* src, double* res) {
        memcpy(src1, src, sizeof(double) * N);
        precondU.upperSolve(src1);
        CTRiC(src1, res);
        precondL.lowerSolve(res);
    };
    int niters = conjugateGradient(N, precondA, Y, X, 100000, 1e-10);
    precondU.upperSolve(X);
    delete[] src1;
#endif

    //fprintf(stderr, "%d iterations\n", niters);

    // get the solution
    double I_overall = V / (X[N - 1] - X[0]);
    for (int i = 0; i < M; i++) {
        double ans = I_overall * invR[i] * (-X[W[i][0]] + X[W[i][1]]);
        printf("%.12lf\n", ans);
    }

    return 0;
}