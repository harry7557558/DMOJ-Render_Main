#error Header "Functions.h" include.


// header
#pragma GCC optimize "Ofast"
#pragma GCC optimize "unroll-loops"
#pragma GCC target "sse,sse2,sse3,sse4,abm,avx,mmx,popcnt"
#include <bits/stdc++.h>
using namespace std;


// iostream fast io
void fast_io() {
	ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
}

// fast integer input
char _, __;
#define scanu(x) do{while((x=getchar())<'0');for(x-='0';'0'<=(_=getchar());x=x*10+_-'0');}while(0)
#define scan(x) do{while((__=getchar())<45);if(__-45)x=__;else x=getchar();for(x-=48;47<(_=getchar());x=x*10+_-48);if(__<46)x=-x;}while(0)

// faster integer input
#define MAX_I 10<<18  // or some other value
char _i[MAX_I], _, __; int i0 = 0;
#define scanu(x) do{while((x=_i[i0++])<'0');for(x-='0';'0'<=(_=_i[i0++]);x=x*10+_-'0');}while(0)
#define scan(x) do{while((__=_i[i0++])<45);if(__-45)x=__;else x=_i[i0++];for(x-=48;47<(_=_i[i0++]);x=x*10+_-48);if(__<46)x=-x;}while(0)
int main() {// initialize
#ifdef __DEBUG
	freopen("stdin.dat", "r", stdin);
	_i[fread(_i, 1, MAX_I, stdin)] = 0;
#else
	_i[fread_unlocked(_i, 1, MAX_I, stdin)] = 0;
#endif
}

// from https://dmoj.ca/user/Dingledooper
#pragma region fast_io
#ifndef __DEBUG
#define getchar getchar_unlocked
#define putchar putchar_unlocked
#define fread fread_unlocked
#define fwrite fwrite_unlocked
#endif
#define INPUT_SIZE 1<<24
#define OUTPUT_SIZE 1<<24
int _i0=0, _o0=0;
char _, _n, __[20], _i[INPUT_SIZE],_o[OUTPUT_SIZE];
#define readin _i[fread(_i, 1, INPUT_SIZE, stdin)]=0
#define writeout fwrite(_o, 1, _o0, stdout)
#define scanu(x) do{while((_=_i[_i0++])<48);for(x=_-48;47<(_=_i[_i0++]);x=x*10+_-48);}while(0)
#define scan(x) do{while((_n=_i[_i0++])<45);if(_n-45)x=_n;else x=_i[_i0++];for(x-=48;47<(_=_i[_i0++]);x=x*10+_-48);if(_n<46)x=-x;}while(0)
#define scanch(x) do{x=_i[_i0++];}while(0)
#define scanch2(x) do{while((_=_i[_i0++])<33);x=_;}while(0)
#define scanstr(s) do{while((_=_i[_i0++])<33);s=_;while(32<(_=_i[_i0++]))s+=_;}while(0)
inline void putnumu(int x) {_=0;do __[_++]=x%10;while(x/=10);while(_--)_o[_o0++]=__[_]+'0';}
inline void putnum(int x) {if(x<0){_o[_o0++]='-';x=-x;}_=0;do __[_++]=x%10;while(x/=10);while(_--)_o[_o0++]=__[_]+'0';}
inline void putnumll(long long x) {if(x<0){_o[_o0++]='-';x=-x;}_=0;do __[_++]=x%10;while(x/=10);while(_--)_o[_o0++]=__[_]+'0';}
inline void putnumull(long long x) {_=0;do __[_++]=x%10;while(x/=10);while(_--)_o[_o0++]=__[_]+'0';}
inline void putstr(std::string s) {for(int i=0;i<int(s.size());i++)_o[_o0++]=s[i];}
inline void putch(char c) {_o[_o0++]=c;}
#define putnl _o[_o0++]='\n'
#pragma endregion



// debug input
// use inside main()
#ifdef _DEBUG
freopen("stdin.dat", "r", stdin);
freopen("stdout.dat", "w", stdout);
freopen("stderr.dat", "w", stderr);
#endif


#pragma region Time and Memory Recording

// time recording
#include <chrono>
std::chrono::high_resolution_clock::time_point _tIMER;
#define start_timer do{_tIMER=std::chrono::high_resolution_clock::now();}while(0)
#define end_timer do{printf("%lfs\n",std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-_tIMER).count());}while(0)


// get memory usage in MB (Windows)
#include <Windows.h>
#include <psapi.h>
double memory_usage() {
	PROCESS_MEMORY_COUNTERS statex;
	GetProcessMemoryInfo(GetCurrentProcess(), &statex, sizeof(statex));
	return statex.WorkingSetSize / 1048576.0;
}

#pragma endregion



