#error Header "DataStructures.h" include.


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
	datatype val;  // sum/prod
	segtree *l, *r;  // children
} R;
void initTree(segtree *R, int a, int b) {
	R->a = a, R->b = b;
	int c = (a + b) / 2;
	if (c == a) {
		R->l = R->r = 0;
		R->val = GlobalArray[a];
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

