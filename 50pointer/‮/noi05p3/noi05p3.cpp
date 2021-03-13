//#pragma GCC optimize "Ofast"
#include <bits/stdc++.h>

using namespace std;

#include <chrono>
auto t0 = std::chrono::high_resolution_clock::now();

typedef struct { int u, v; } ivec2;


struct piece {
	char id;
	std::vector<ivec2> bs;
	piece(char id, std::initializer_list<std::initializer_list<bool>> s) : id(id) {
		bool x[4][4];
		for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
			if (x[i][j] = *((s.begin() + i)->begin() + j)) bs.push_back(ivec2{ i, j });
		}
	}
	/*void printpiece() {
		for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) printf("%c%c", x[i][j] ? '#' : ' ', j == 3 ? '\n' : ' ');
	}*/
};

std::vector<piece> P;

void init() {
	// what the **ck
	P.push_back(piece('H', {
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('A', {
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('F', {
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0}
		}));
	P.push_back(piece('I', {
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0}
		}));
	P.push_back(piece('K', {
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('B', {
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0}
		}));
	P.push_back(piece('A', {
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('A', {
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('A', {
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('B', {
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{1, 1, 1, 0},
		{1, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{1, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{1, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('C', {
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('D', {
		{1, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('E', {
		{1, 1, 1, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('E', {
		{1, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('E', {
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('E', {
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('F', {
		{1, 1, 1, 1},
		{0, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('F', {
		{1, 1, 1, 1},
		{0, 0, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('F', {
		{0, 1, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('F', {
		{0, 0, 1, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('F', {
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{1, 0, 0, 0}
		}));
	P.push_back(piece('F', {
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0}
		}));
	P.push_back(piece('F', {
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0}
		}));
	P.push_back(piece('G', {
		{1, 1, 1, 0},
		{1, 0, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('G', {
		{1, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('G', {
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('G', {
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('H', {
		{1, 1, 0, 0},
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('H', {
		{1, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('H', {
		{1, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('H', {
		{1, 1, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('H', {
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('H', {
		{0, 1, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('H', {
		{1, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('I', {
		{1, 1, 1, 0},
		{0, 0, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('I', {
		{1, 1, 0, 0},
		{0, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('I', {
		{0, 1, 1, 1},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('I', {
		{0, 0, 1, 1},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('I', {
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0}
		}));
	P.push_back(piece('I', {
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0}
		}));
	P.push_back(piece('I', {
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{1, 0, 0, 0}
		}));
	P.push_back(piece('J', {
		{0, 1, 0, 0},
		{1, 1, 1, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('K', {
		{0, 0, 1, 0},
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('K', {
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('K', {
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('L', {
		{1, 1, 1, 1},
		{1, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('L', {
		{1, 1, 1, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('L', {
		{1, 0, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('L', {
		{0, 0, 0, 1},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
		}));
	P.push_back(piece('L', {
		{1, 1, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0}
		}));
	P.push_back(piece('L', {
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 1, 0, 0}
		}));
	P.push_back(piece('L', {
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0}
		}));
	P.push_back(piece('L', {
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 0, 0}
		}));

	/*for (piece p : P) {
	p.printpiece();
	printf("\n");
	}
	printf("%d\n", P.size());*/
}


char board[20][20];

bool used[128];

void print() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j <= i; j++) {
			putchar(board[i][j] ? board[i][j] : '.');
		}
		putchar('\n');
	}
}


int dp[20][20];
int area_size[100];

// disjoint set
int parent[256];  // all initialize to -1
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
	parent[j_rep] = i_rep;
	return true;
}



bool isPossible() {
	//print();
	int cc = 1;
	for (int i = 0; i < 40; i++) parent[i] = -1, area_size[i] = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j <= i; j++) {
			if (!board[i][j]) {
				bool hasTop = i != 0 && !board[i - 1][j];
				bool hasLeft = j != 0 && !board[i][j - 1];
				if (!hasLeft && !hasTop) {
					dp[i][j] = cc;
					area_size[cc] = 0; cc++;
				}
				if (!hasLeft && hasTop) {
					dp[i][j] = dp[i - 1][j];
				}
				if (hasLeft && !hasTop) {
					dp[i][j] = dp[i][j - 1];
				}
				if (hasLeft && hasTop) {
					dp[i][j] = dp[i][j - 1];
					if (dp[i - 1][j] != dp[i][j - 1]) {
						area_size[dp[i][j - 1]] += area_size[dp[i - 1][j]];
						unionSet(dp[i][j - 1], dp[i - 1][j]);
						area_size[dp[i - 1][j]] = 0;
					}
				}
				area_size[findRepresentative(dp[i][j])]++;
			}
		}
	}
	for (int i = 1; i < cc; i++) {
		int sz = area_size[findRepresentative(i)];
		if (sz < 3) return false;
		if (sz == 3 && used['A']) return false;
		if (sz == 4 && (used['B'] && used['C'] && used['D'])) return false;
		if (sz == 6) return false;
		if (sz == 7 && (used['A'] || (used['B'] && used['C'] && used['D']))) return false;
		//if (sz == 8 && (((int)used['B'] + (int)used['C'] + (int)used['D']) < 2 && used['A'])) return false;
		if (sz == 9 && (used['B'] && used['C'] && used['D'])) return false;
		//if (sz < 10) printf("%d ", sz);
	}
	return true;
}


int call_count = 0;

void bruteforce() {
	call_count++;

	char letter = 0;
	for (char i: string("AHBCDEFGIJKL")) {
		if (!used[i]) { letter = i; break; }
	}
	if (letter == 0) {
		// success
		print();
#ifndef __DEBUG
		exit(0);
#endif
	}

#ifndef __DEBUG
	double tle = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
	if (tle > 0.995) exit(0 * printf("No solution\n"));
#endif

	for (int pi = 0; pi < (int)P.size(); pi++) {
		if (P[pi].id == letter) {

			for (int i = 0; i < 10; i++) for (int j = 0; j <= i; j++) {

				// test if the piece can be placed at this location
				bool ok = true;
				for (ivec2 uv : P[pi].bs) {
					if (board[i + uv.u][j + uv.v]) { ok = false; break; }
				}
				if (!ok) continue;

				// put the piece there and search recursively
				for (ivec2 uv : P[pi].bs) {
					board[i + uv.u][j + uv.v] = letter;
				}
				used[letter] = true;
				if (1) {
					if (isPossible()) bruteforce();
				}
				else bruteforce();
				used[letter] = false;
				for (ivec2 uv : P[pi].bs) {
					board[i + uv.u][j + uv.v] = 0;
				}

			}
		}
	}
}


int main() {
#ifdef __DEBUG
	freopen("stdin.dat", "r", stdin);
#endif
	ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

	init();

	for (int i = 0; i < 20; i++) for (int j = 0; j < 20; j++) board[i][j] = (char)-1;
	for (int i = 0; i < 128; i++) used[i] = false;

	for (int i = 0; i < 10; i++) {
		string s; cin >> s;
		for (int j = 0; j <= i; j++) {
			board[i][j] = s[j] == '.' ? 0 : s[j];
			used[s[j]] = true;

			//board[i][j] = 0, used[s[j]] = false;
		}
	}
	/*for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) printf("%d", board[i][j]);
		printf("\n");
	}*/



	bruteforce();
	double tle = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();

#ifdef __DEBUG
	printf("%lfsecs\n", tle);
	printf("%d\n", call_count);
#endif

	printf("No solution\n");


	return 0;
}
