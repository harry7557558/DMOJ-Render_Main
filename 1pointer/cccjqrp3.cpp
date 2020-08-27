#include <iostream>
using namespace std;

int main() {
	unsigned long long A, B, C;
	cin >> A >> B >> C;
	cout << ((A + B + C) % 42069900169420) << endl;
	return 0;
}