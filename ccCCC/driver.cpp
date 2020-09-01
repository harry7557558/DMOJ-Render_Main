#if 0
#define BUILD
#endif

#ifndef BUILD
#define main __main
#endif
#include "CCC.cpp"
#undef main

#ifndef BUILD
#include <crtdbg.h>
#include <cstdlib>
int main() {
	_CrtSetBreakAlloc(-1);
#if 1
	const int argc = 3;
	const char* argv[argc] = { __FILE__, "" };
	__main(argc, const_cast<char**>(argv));
#else
	__main();
#endif
	_CrtDumpMemoryLeaks();
	return 0;
}
#endif
