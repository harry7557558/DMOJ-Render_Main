#include <stdio.h>
#define print(...) printf(#__VA_ARGS__)
int main() { print(Hello, World!); }