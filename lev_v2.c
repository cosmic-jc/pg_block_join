#include "similarity.h"

int _lev_v2(char *a, char *b) {
  int      *arow, *brow, *trow;
  int      alen, blen;
  int      i, j;
  int      res;

  alen = strlen(a);
  blen = strlen(b);

  if (alen == 0) return blen;
  if (blen == 0) return alen;

  arow = (int *) malloc((blen + 1) * sizeof(int));
  brow = (int *) malloc((blen + 1) * sizeof(int));

  for (i = 0; i < alen; i++)
    a[i] = tolower(a[i]);
  for (j = 0; j < blen; j++)
    b[j] = tolower(b[j]);

  for (i = 0; i <= blen; i++)
    arow[i] = i;

  for (i = 1; i <= alen; i++) {
    brow[0] = i;

    for (j = 1; j <= blen; j++) {
      int scost = (a[i - 1] == b[j - 1]) ? 0 : 1;
      brow[j] = min3(brow[j - 1] + 1, arow[j] + 1, arow[j - 1] + scost);
    }

    trow = arow;
    arow = brow;
    brow = trow;

  }

  res = arow[blen];

  free(arow);
  free(brow);

  return res;
}

PG_FUNCTION_INFO_V1(lev_v2);

Datum
lev_v2(PG_FUNCTION_ARGS) {
  char *a, *b;

  a = DatumGetPointer(DirectFunctionCall1(textout,
                                          PointerGetDatum(PG_GETARG_TEXT_P(0))));
  b = DatumGetPointer(DirectFunctionCall1(textout,
                                          PointerGetDatum(PG_GETARG_TEXT_P(1))));


  PG_RETURN_INT32(_lev_v2(a, b));
}
