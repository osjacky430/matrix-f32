/**
 * @Date:   2019-11-12T14:52:29+08:00
 * @Last modified time: 2019-11-16T11:34:34+08:00
 */

#pragma once

void runtimeErrorReset(void);
char const* runtimerErrorGetLastError(void);
int runtimeErrorGetLastParam(void);

void runtimeErrorThrow(const char* message, int param, const char* file, int line);

#define THROW(description, param) runtimeErrorThrow(description, param, __FILE__, __LINE__)
