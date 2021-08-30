/*
  National Olympiad in Informatics, China, 2010
  Day 2, Problem 3 - Happily Growing

  Solution Grader by Alex Li <alextrovert@gmail.com>
*/

#include <cstdio>
#include <cmath>

//#define return(x) { printf("Returned %d\n", x); return x; }

const int MAX_N = 10005;
const double EPS = 1E-4;

#define EQ(a, b) (fabs((a) - (b)) <= EPS)           /* equal to */
#define LT(a, b) ((a) < (b) - EPS)                 /* less than */
#define GT(a, b) ((a) > (b) + EPS)              /* greater than */
#define LE(a, b) ((a) <= (b) + EPS)    /* less than or equal to */
#define GE(a, b) ((a) >= (b) - EPS) /* greater than or equal to */

inline double dist(double x1, double y1, double x2, double y2) {
  return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

int n, k;
double V, T, w[MAX_N];
double x[MAX_N], y[MAX_N];
double p[MAX_N], q[MAX_N];

int main(int argc, char* argv[]) {
  FILE *fans = fopen(argv[1], "r");
  FILE *fout = fopen(argv[2], "r");
  FILE *fin  = fopen(argv[3], "r");

  fscanf(fin, "%*d"); //discard test case number
  fscanf(fin, "%lf%lf%lf%lf%lf", w, &V, &T, x, y);
  fscanf(fin, "%d", &n);
  for (int i = 1; i <= n; i++) {
    fscanf(fin, "%lf%lf%lf%lf%lf", w+i, x+i, y+i, p+i, q+i);
  }

  double w_user, t = 0, new_x, new_y;
  double curr_t = t, curr_w = w[0], curr_x = x[0], curr_y = y[0];
  int s;
  bool eaten[MAX_N] = {0};

  if (fscanf(fout, "%d%lf", &k, &w_user) != 2)
    return(1);
  for (int i = 0; i < k; i++) {
    if (fscanf(fout, "%lf%lf%lf%d", &t, &new_x, &new_y, &s) != 4)
      return(1);
    if (s < 1 || s > n || eaten[s] || GE(w[s], curr_w))
      return(1);
    if (LT(t, curr_t) || GT(t, T))
      return(1);
    if (!EQ(new_x, x[s] + p[s]*t) || !EQ(new_y, y[s] + q[s]*t))
      return(1);
    
    if (EQ(t, curr_t)) { //two shrimps at the same place (???)
      if (!EQ(curr_x, new_x) && !EQ(curr_y, new_y))
        return(1);
    } else { 
      double v = dist(curr_x, curr_y, new_x, new_y) / (t - curr_t);
      if (GT(v, V)) return(1); //cannot move this fast
      curr_t = t;
      curr_x = new_x;
      curr_y = new_y;
    }
    eaten[s] = true;
    curr_w += w[s];
  }

  if (!EQ(curr_w, w_user + w[0])) return(1); //weights don't add up

  double a;
  for (int i = 10; i >= 2; i--) {
    fscanf(fans, "%lf", &a);
    if (GE(w_user, a)) {
      printf("%d\n10\n", i);
      return(0);
    }
  }
  if (GT(w_user, 0.0)) {
    printf("1\n10\n");
    return(0);
  }
  return(1);
}
