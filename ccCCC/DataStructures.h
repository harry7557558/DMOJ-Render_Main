#error Header "DataStructures.h" include.


#define MAXN 1000000
int N;



// binary indexed tree / Fenwick tree
int BIT[MAXN + 1];  // all elements are zero at first
void updateBIT(int i, int d) {  // increase the ith element by d
	i++;
	while (i <= N) BIT[i] += d, i += i & (-i);
}
int getsumBIT(int i) {  // return the sum from 0 to i, inclusive
	i++;
	int s = 0;
	while (i > 0) s += BIT[i], i -= i & (-i);
	return s;
}


// segment tree
struct segtree {
	int a, b;  // [a,b)
	datatype val;  // sum/prod of int/mat/algebra structure
	segtree *l, *r;  // children
} R;
void initTree(segtree *R, int a, int b) {
	R->a = a, R->b = b;
	int c = (a + b) / 2;
	if (c == a) {
		R->l = R->r = 0;
		R->val = Arr[a];
	}
	else {
		R->l = new segtree; initTree(R->l, a, c);
		R->r = new segtree; initTree(R->r, c, b);
		R->val = R->r->val + R->l->val;  // sum or prod
	}
}
datatype queryTree(const segtree *R, int a, int b) {  // [a,b)
	if (R->a >= a && R->b <= b) return R->val;  // completely inside the interval
	if (R->a >= b || R->b <= a) return unit_element;  // completely outside the interval
	return queryTree(R->r, a, b) + queryTree(R->l, a, b);  // sum or prod
}


// disjoint set
int parent[MAXN];  // all initialize to -1
uint8_t rank[MAXN];  // all initialize to 0
int findRepresentative(int i) {
	if (parent[i] == -1) return i;
	else {
		int ans = findRepresentative(parent[i]);
		parent[i] = ans;
		return ans;
	}
}
bool unionSet(int i, int j) {
	int i_rep = findRepresentative(i);
	int j_rep = findRepresentative(j);
	if (i_rep == j_rep) return false;
#if 0
	parent[i_rep] = j_rep;
#else
	if (rank[i_rep] < rank[j_rep])
		parent[i_rep] = j_rep;
	else if (rank[i_rep] > rank[j_rep])
		parent[j_rep] = i_rep;
	else parent[j_rep] = i_rep, rank[i_rep]++;
#endif
	return true;
}


