#include <bits/stdc++.h>
using namespace std;


// vector template
struct vec2 {
	double x, y;
	vec2() {}
	explicit vec2(double a) :x(a), y(a) {}
	vec2(double x, double y) :x(x), y(y) {}
	bool operator == (vec2 p) const { return x == p.x && y == p.y; }
	bool operator != (vec2 p) const { return x != p.x || y != p.y; }
	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (vec2 v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (vec2 v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (vec2 v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator * (double a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; }
	friend double length(vec2 v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(vec2 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	friend double dot(vec2 u, vec2 v) { return u.x*v.x + u.y*v.y; }
	friend double det(vec2 u, vec2 v) { return u.x*v.y - u.y*v.x; }
	vec2 rot() const { return vec2(-y, x); }
};


// global constants
int ID;  // test case ID
double W0;  // Nemo's initial weight
double maxV;  // Nemo's maximum speed
double maxT;  // Nemo's time to eat shrimp
vec2 P0;  // Nemo's initial position
int SN;  // number of shrimps
struct shrimp {
	int id;  // index
	vec2 p;  // initial position
	vec2 v;  // velocity
	double w;  // weight
} S[10000];  // shrimps

// global variables
double W;  // Nemo's weight
double T;  // time
vec2 P;  // position
struct EatEvent {
	int s;  // shrimp ID
	double t;  // time
	vec2 p;  // position
};
vector<EatEvent> eaten;  // eaten shrimps
bool hasBeenEaten[10000];  // has been eaten


// calculate the additional time range Nemo may need to reach a shrimp
bool timeToEatShrimp(vec2 p0, shrimp s, double t, double &t0, double &t1) {
	vec2 p = s.p + s.v*t - p0, v = s.v;
	// (p+v*t)^2 <= (maxV*t)^2
	double a = dot(v, v) - maxV * maxV, b = dot(p, v), c = dot(p, p);
	if (a <= 0.0) { // greater than a number
		t0 = (-sqrt(b*b - a * c) - b) / a;
		if (t0 < 0.0) t0 = 0.0;
		t1 = INFINITY;
		return true;
	}
	else {
		double delta = b * b - a * c;
		if (delta < 0.0) return false;
		delta = sqrt(delta);
		t0 = (-sqrt(delta) - b) / a;
		t1 = (sqrt(delta) - b) / a;
		if (t0 < 0.0) t0 = 0.0;
		if (t1 < 0.0) return false;
		return true;
	}
}



// Test case 1 and 3
// Eat in order of increasing weight/index
namespace EatInOrder {
	void main() {
		for (int _ = 0; _ < SN; _++) {
			int eat_id = -1;
			for (int i = 0; i < SN; i++) {
				if (hasBeenEaten[i]) continue;
				double t0, t1;
				if (W > S[i].w && timeToEatShrimp(P, S[i], T, t0, t1) && T + t0 < maxT) {
					EatEvent e;
					e.s = i;
					e.t = T + t0;
					e.p = S[i].p + S[i].v * e.t;
					eaten.push_back(e);
					W += S[i].w;
					P = e.p;
					T += t0;
					hasBeenEaten[i] = true;
					eat_id = i;
					break;
				}
			}
			if (eat_id == -1) break;
		}
	}
}


// Test case 2
// Brute force search all permutations
namespace BruteForce {
	double W_temp;
	vec2 P_temp;
	double T_temp;
	vector<EatEvent> eaten_temp;

	void recurse() {
		for (int i = 0; i < SN; i++) if (!hasBeenEaten[i]) {
			double W_temp0 = W_temp;
			vec2 P_temp0 = P_temp;
			double T_temp0 = T_temp;
			double t0, t1;
			if (W_temp > S[i].w && timeToEatShrimp(P_temp, S[i], T_temp, t0, t1) && T_temp + t0 < maxT) {
				hasBeenEaten[i] = true;
				EatEvent e;
				e.s = i;
				e.t = T_temp + t0;
				e.p = S[i].p + S[i].v * e.t;
				W_temp += S[i].w;
				P_temp = e.p;
				T_temp += t0;
				eaten_temp.push_back(e);
				if (W_temp > W) {
					W = W_temp;
					eaten = eaten_temp;
				}
				recurse();
				eaten_temp.pop_back();
			}
			hasBeenEaten[i] = false;
			W_temp = W_temp0, P_temp = P_temp0, T_temp = T_temp0;
		}
	}

	void main() {
		W_temp = W0;
		P_temp = P0;
		T_temp = 0.0;
		recurse();
	}
}


// Test case 9
// Always eat the shrimp that is fastest to reach
namespace Greedy {
	void main() {
		for (int _ = 0; _ < SN; _++) {
			int eat_id = -1;
			double min_t0 = INFINITY;
			for (int i = 0; i < SN; i++) {
				if (hasBeenEaten[i]) continue;
				double t0, t1;
				if (W > S[i].w && timeToEatShrimp(P, S[i], T, t0, t1) && T + t0 < maxT) {
					if (t0 < min_t0) eat_id = i, min_t0 = t0;
				}
			}
			if (eat_id == -1) break;

			EatEvent e;
			e.s = eat_id;
			e.t = T + min_t0;
			e.p = S[eat_id].p + S[eat_id].v * e.t;
			eaten.push_back(e);
			W += S[eat_id].w;
			P = e.p;
			T += min_t0;
			hasBeenEaten[eat_id] = true;
		}
	}
}


// Test case 10 ??
// Eat the shrimp with the minimum cost where the cost function involves random number
// Judge feedback: maximum weight 21848713
namespace RandomizedGreedy {
	uint32_t seed;
	inline uint32_t lcg_next() {
		return seed = seed * 1664525u + 1013904223u;
	}
	double random(double a, double b) {
		return a + (b - a) * lcg_next() * (1. / 4294967296.);  // [0,1)
	}

	double cost_function(double w, double t, double d) {
		return 1.0 / (w / (t*t) + 1.0 / (random(1.0, 10.0)*d) + random(1.0, 10.0));
	}

	void randomTest() {
		double W_temp = W0;
		vec2 P_temp = P0;
		double T_temp = 0.0;
		vector<EatEvent> eaten_temp;
		for (int _ = 0; _ < SN; _++) hasBeenEaten[_] = false;

		for (int _ = 0; _ < SN; _++) {
			int eat_id = -1;
			double min_t0 = INFINITY;
			double min_cost = INFINITY;
			for (int i = 0; i < SN; i++) {
				if (hasBeenEaten[i]) continue;
				double t0, t1;
				if (W_temp > S[i].w && timeToEatShrimp(P_temp, S[i], T_temp, t0, t1) && T_temp + t0 < maxT) {
					double cost = cost_function(S[i].w, t0, length(S[i].p + S[i].v*(T_temp + t0) - P_temp));
					if (cost < min_cost) eat_id = i, min_t0 = t0, min_cost = cost;
				}
			}
			if (eat_id == -1) break;

			EatEvent e;
			e.s = eat_id;
			e.t = T_temp + min_t0;
			e.p = S[eat_id].p + S[eat_id].v * e.t;
			eaten_temp.push_back(e);
			W_temp += S[eat_id].w;
			P_temp = e.p;
			T_temp += min_t0;
			hasBeenEaten[eat_id] = true;
		}
		if (W_temp > W) {
			W = W_temp;
			eaten = eaten_temp;
		}
	}

	// find the best random number seed
	void findSeed() {
		double maxW = W0;
		int maxSeed = 0;
		for (int s = 0; s < 10; s++) {
			seed = (uint32_t)s;
			W = 0.0;
			randomTest();
			printf("%d %lf ", s, W);
			if (W > maxW) {
				printf("Yeah!");
				maxW = W, maxSeed = s;
			}
			printf("\n");
		}
		printf("Best seed: %d\n", maxSeed);
	}

	void main() {
		//findSeed();
		seed = 2;  // 22003249
		randomTest();
	}
}


// Test case 7 and 8
// Shrimps are arranged in a triangle, not moving
// Nemo should eat only one shrimp in each layer of the triangle
// (time limit is exactly the time for Nemo to reach the last layer)
namespace Triangle {
	int N;
	int ids[60][60];
	vec2 pos[60][60];
	double weight[60][60];
	double max_weight[60][60];  // dp

	void main() {
		N = ID == 7 ? 15 : 60;
		int id = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j <= i; j++) {
				pos[i][j] = S[id].p;
				ids[i][j] = id;
				weight[i][j] = S[id].w;
				id++;
			}
		}

		// DP
		max_weight[0][0] = weight[0][0];
		for (int i = 1; i < N; i++) {
			for (int j = 0; j <= i; j++) {
				max_weight[i][j] = 0.0;
				if (j != 0) max_weight[i][j] = max(max_weight[i][j], max_weight[i - 1][j - 1]);
				if (j != i) max_weight[i][j] = max(max_weight[i][j], max_weight[i - 1][j]);
				max_weight[i][j] += weight[i][j];
			}
		}
		struct ivec2 {
			int i, j;
		};
		W = 0.0;
		int j = -1;
		for (int i = 0; i < N; i++) {
			if (max_weight[N - 1][i] > W) j = i, W = max_weight[N - 1][i];
		}
		vector<ivec2> path;
		path.push_back(ivec2{ N - 1, j });
		for (int i = N - 1; i > 0; i--) {
			if (j != 0 && max_weight[i][j] - weight[i][j] == max_weight[i - 1][j - 1]) j = j - 1;
			else if (j != i && max_weight[i][j] - weight[i][j] == max_weight[i - 1][j]) j = j;
			else throw("bug");
			path.push_back(ivec2{ i - 1, j });
		}
		W += W0;

		// Convert to path
		vec2 p0 = P0;
		for (int _ = N - 1; _ >= 0; _--) {
			int i = path[_].i, j = path[_].j;
			vec2 p1 = pos[i][j];
			double dt = length(p1 - p0) / maxV;
			EatEvent e;
			e.p = p1;
			e.s = ids[i][j];
			e.t = T + dt;
			T = e.t;
			if (T > maxT) throw(T);
			if (S[e.s].p != e.p) throw(e.t);
			eaten.push_back(e);
			p0 = p1;
		}
	}
}


// Test case 5 and 6
// Shrimps are close in horizontal direction but moving fast in verticle direction
// Nemo should only move horizontally to get hit by shrimps
namespace Vertical {
	struct shrp {
		int id;  // index
		double x;  // x-coordinate
		double t;  // time to hit horizontal axis
		double w;  // weight
	} Ss[10000];

	double maxWeight[10000];  // maximum weight after first i shrimps come and ith shrimp get eaten
	int prevI[10000];  // used to backtrack maxWeight

	void main() {
		for (int i = 0; i < SN; i++) {
			Ss[i].id = i;
			Ss[i].x = S[i].p.x;
			Ss[i].t = -S[i].p.y / S[i].v.y;
			Ss[i].w = S[i].w;
		}

		// DP
		std::sort(Ss, Ss + SN, [](shrp a, shrp b) { return a.t < b.t; });
		for (int j = 0; j < SN; j++) {
			double maxw = 0.0;
			int maxi = -1;
			double min_t = abs(Ss[j].x - P.x) / maxV;
			if (Ss[j].t >= min_t && W0 > Ss[j].w) {
				maxw = W0 + Ss[j].w;
				maxi = -1;
			}
			for (int i = 0; i < j; i++) {
				double min_t = abs(Ss[j].x - Ss[i].x) / maxV;
				if (Ss[j].t - Ss[i].t >= min_t && maxWeight[i] > Ss[j].w) {
					if (maxWeight[i] + Ss[j].w > maxw) {
						maxw = maxWeight[i] + Ss[j].w;
						maxi = i;
					}
				}
			}
			maxWeight[j] = maxw;
			prevI[j] = maxi;
		}
		W = W0;
		int maxi = -1;
		for (int i = 0; i < SN; i++) {
			if (maxWeight[i] > W) W = maxWeight[i], maxi = i;
		}
		vector<int> backtrack;
		for (; maxi != -1; maxi = prevI[maxi]) {
			backtrack.push_back(maxi);
		}
		for (int _ = (int)backtrack.size(); _--;) {
			int i = backtrack[_];
			EatEvent e;
			e.s = Ss[i].id;
			e.t = Ss[i].t;
			e.p = vec2(Ss[i].x, 0.0);
			eaten.push_back(e);
		}
	}
}


// Test case 4
// All shrimps are on a straight line, not moving, range from [-4995,0)U(0,5005]; Nemo is initially at (0,0)
// Shrimps around Nemo have weight growing in the power of 2
// Nemo should first eat those shrimps, then eat along an interval
namespace Horizontal {
	constexpr int DX = 4995;  // add the x coordinate of each shrimp by this number
	int id[10001];  // ID of the shrimp at each position
	double weight[10001];  // weight of the shrimp at each position
	double max_w = 0.0;  // maximum weight of a shrimp
	struct eat {
		int id;
		int t;
	};  // to be converted to EatEvent

	void main() {
		int max_dist = (int)(maxT / maxV);  // 5000
											// initialize id and weight
		for (int i = 0; i <= 10000; i++) id[i] = -1;
		for (int i = 0; i < SN; i++) {
			int x = (int)S[i].p.x + DX;
			id[x] = i, weight[x] += S[i].w;
		}
		for (int i = 0; i <= 10000; i++) max_w = max(max_w, weight[i]);  // 12582912 = 6*(2**21)
																		 // keep growing weight until you eat the shrimp with weight 12582912
		vector<eat> before_eat_list;
		int x0 = DX;
		W = W0; int T = 0;
		int next_w = 6;
		while (W < max_w) {
			// search both sides
			int dx;
			for (dx = 1;; dx++) {
				int l = x0 - dx, r = x0 + dx;
				if (!(l >= 0 || r <= 10000)) break;
				if (l >= 0 && id[l] != -1 && weight[l] == next_w) {
					x0 = l, T += dx;
					before_eat_list.push_back(eat{ id[l], T });
					id[l] = -1, W += weight[l];
					break;
				}
				else if (r <= 10000 && id[r] != -1 && weight[r] == next_w) {
					x0 = r, T += dx;
					before_eat_list.push_back(eat{ id[r], T });
					id[r] = -1, W += weight[r];
					break;
				}
			}
			next_w *= 2;  // test input: 6, 12, 24, 48, 96, ..., 12582912
		}
		// first go left, then go right
		max_dist -= T;
		double max_weight = 0.0;
		vector<eat> max_eat_list;
		for (int left = max(x0 - max_dist, 0); left <= x0; left++) {
			int right = min(left + (max_dist - (x0 - left)), 10001);
			double tot_weight = W;
			vector<eat> eat_list; eat_list.reserve(max_dist + 1);
			int t = 0;
			for (int x = x0; x >= left; x--, t++) {
				if (id[x] != -1 && weight[x] < tot_weight) {
					tot_weight += weight[x];
					eat_list.push_back(eat{ id[x], T + t });
					id[x] = -1;  // eaten
				}
			}
			t--;
			for (int x = left; x <= right; x++, t++) {
				if (id[x] != -1 && weight[x] < tot_weight) {
					tot_weight += weight[x];
					eat_list.push_back(eat{ id[x], T + t });
					id[x] = -1;
				}
			}
			t--;
			if (tot_weight > max_weight) {
				max_weight = tot_weight;
				max_eat_list = eat_list;
			}
		}
		// first go right, then go left
		{
			// not needed for this test case
		}
		// result
		W = max_weight;
		vector<eat> eat_list = before_eat_list;
		eat_list.insert(eat_list.end(), max_eat_list.begin(), max_eat_list.end());
		for (eat et : eat_list) {
			EatEvent e;
			e.s = et.id;
			e.p = S[et.id].p;
			e.t = et.t;
			eaten.push_back(e);
		}
	}
}




// for debugging purpose, check if solution is valid
void checkOutput() {
	const double eps = 1e-8;
	vec2 p = P0;
	double t = 0.0;
	double w = W0;
	for (int i = 0; i < SN; i++) hasBeenEaten[i] = false;
	if (eaten.size() == 0)
		fprintf(stderr, "No shrimp has been eaten\n");
	for (int i = 0; i < (int)eaten.size(); i++) {
		EatEvent e = eaten[i];
		if (hasBeenEaten[e.s])
			fprintf(stderr, "%d: Eat a shrimp that is already eaten\n", i);
		hasBeenEaten[e.s] = true;
		if (w < S[e.s].w)
			fprintf(stderr, "%d: Eat a shrimp greater than Nemo's weight\n", i);
		w += S[e.s].w;
		vec2 q = S[e.s].p + S[e.s].v * e.t;
		if (max(abs(q.x - e.p.x), abs(q.y - e.p.y)) > eps)
			fprintf(stderr, "%d: Wrong position\n", i);
		if (length(q - p) > maxV * (e.t - t) + eps)
			fprintf(stderr, "%d: Nemo cannot travel that fast\n", i);
		p = q;
		t = e.t;
		if (t > maxT)
			fprintf(stderr, "%d: TLE\n", i);
	}
	if (abs(w - W) > eps)
		fprintf(stderr, "Incorrect W value");
}


#ifdef __DEBUG
namespace Grader {
#include "noi10p6/grader.cpp"
}
#endif


#define NOI 0  // if submitting on the Chinese OJ

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(NULL); std::cout.tie(NULL);
	FILE* output_stream = stdout;
#ifdef __DEBUG
	const std::string in = "nemo10.in", out = "nemo10.out";
	freopen(&("noi10p6/" + in)[0], "r", stdin);
	output_stream = fopen(&("noi10p6/" + out)[0], "wb");
#endif

	// input
#if !NOI
	cin >> ID;
#endif
	cin >> W0 >> maxV >> maxT >> P0.x >> P0.y;
	cin >> SN;
	for (int i = 0; i < SN; i++) {
		S[i].id = i;
		cin >> S[i].w >> S[i].p.x >> S[i].p.y >> S[i].v.x >> S[i].v.y;
	}
#if NOI
	if (SN == 2) ID = 1;
	if (SN == 8) ID = 2;
	if (SN == 30) ID = 3;
	if (SN == 10000) {
		if (W0 == 7) ID = 4;
		if (S[0].w == 2347) ID = 5;
		if (S[0].w == 7957) ID = 6;
	}
	if (SN == 120) ID = 7;
	if (SN == 1830) ID = 8;
	if (SN == 1000) ID = 9;
	if (SN == 5000) ID = 10;
#endif
	W = W0;
	T = 0.0;
	P = P0;

	// solve
	if (ID == 1 || ID == 3) EatInOrder::main();
	if (ID == 2) BruteForce::main();
	if (ID == 7 || ID == 8) Triangle::main();
	if (ID == 5 || ID == 6) Vertical::main();
	if (ID == 4) Horizontal::main();
	if (ID == 9) Greedy::main();
	if (ID == 10) RandomizedGreedy::main();

	// check
#ifdef __DEBUG
	checkOutput();
#endif

	// output
	fprintf(output_stream, "%d\n", (int)eaten.size());
	fprintf(output_stream, "%.10lf\n", W - W0);
	for (EatEvent e : eaten) {
		fprintf(output_stream, "%.10lf %.10lf %.10lf %d\n", e.t, e.p.x, e.p.y, e.s + 1);
	}

	// check
#ifdef __DEBUG
	fclose(output_stream);
	const char* argv[4] = {
		"",
		"noi10p6/nemo10.ans",
		"noi10p6/nemo10.out",
		"noi10p6/nemo10.in",
	};
	int sol = Grader::main(4, const_cast<char**>(argv));
	fprintf(stderr, "%d\n", sol);
#endif
	return 0;
}
