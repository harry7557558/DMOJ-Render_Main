#include <bits/stdc++.h>
using namespace std;

int s(string s) {
	if (s == "one" || s == "un" || s == "一") return 1;
	if (s == "two" || s == "deux" || s == "二") return 2;
	if (s == "three" || s == "trois" || s == "三") return 3;
	if (s == "four" || s == "quatre" || s == "四") return 4;
	if (s == "five" || s == "cinq" || s == "五") return 5;
	if (s == "six" || s == "six" || s == "六") return 6;
	if (s == "seven" || s == "sept" || s == "七") return 7;
	if (s == "eight" || s == "huit" || s == "八") return 8;
	if (s == "nine" || s == "neuf" || s == "九") return 9;
	if (s == "ten" || s == "dix" || s == "十") return 10;
	return stoi(s);
}

int main() {
	int N; cin >> N;
	while (N--) {
		string a, b; cin >> a >> b;
		cout << (s(a) + s(b)) << endl;
	}
	return 0;
}
