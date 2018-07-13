#ifndef SIMILARITY_H
#define SIMILARITY_H

#include "postgres.h"
#include "utils/builtins.h"

#define min3(a, b, c) ((a < b && a < c) ? a : ((b < c) ? b : c))

int _lev_v1(char *a, char *b);
int _lev_v2(char *a, char *b);
int _lev_v3(const char *source, int slen, const char *target, int tlen);
int _lev_v4(const char *source, int slen, const char *target, int tlen, int maxd);
bool _lev_v5(const char *source, int slen, const char *target, int tlen, int maxd);
float4 _jac2(const char* s, int m, const char* t, int n);

#endif
