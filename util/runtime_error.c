/**
 * @Date:   2019-11-12T14:52:29+08:00
 * @Last modified time: 2019-11-12T21:40:22+08:00
 */
#include "runtime_error.h"

#include <assert.h>
#include <stdbool.h>

static char const *message = "No Error";
static int parameter = -1;
static char const *file = 0;
static int line = -1;

void runtimeErrorReset(void) {
	message = "No Error";
	parameter = -1;
}

char const *runtimerErrorGetLastError(void) { return message; }

int runtimeErrorGetLastParam(void) { return parameter; }

void runtimeErrorThrow(const char *msg, int param, const char *f, int l) {
	message = msg;
	parameter = param;
	file = f;
	line = l;

	assert(false);
}
