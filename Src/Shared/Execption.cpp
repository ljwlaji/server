#include "Execption.h"

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <cstdarg>

void Execption::Assert(char const * file, int line, char const * function, char const * message)
{
	fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s\n",
		file, line, function, message);
	*((volatile int*)nullptr) = 0;
	exit(1);
}

void Execption::Assert(char const * file, int line, char const * function, char const * message, char const * format, ...)
{
	va_list args;
	va_start(args, format);

	fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s ", file, line, function, message);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	fflush(stderr);

	va_end(args);
	*((volatile int*)nullptr) = 0;
	exit(1);
}
