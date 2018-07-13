#include "similarity.h"

int _lev_v1(char *a, char *b) {
  int      **matrix;
  int      alen, blen;
  int      i, j;
  int      res;

  alen = strlen(a);
  blen = strlen(b);


  if (alen == 0)
    return blen;
  if (blen == 0)
    return alen;

  matrix = (int **) malloc((alen + 1) * sizeof(int *));

  for (i = 0; i <= alen; i++)
    matrix[i] = (int *) malloc((blen + 1) * sizeof(int));

  for (i = 0; i < alen; i++)
    a[i] = tolower(a[i]);
  for (j = 0; j < blen; j++)
    b[j] = tolower(b[j]);

  for (i = 0; i <= alen; i++)
    matrix[i][0] = i;
  for (j = 0; j <= blen; j++)
    matrix[0][j] = j;

  for (i = 1; i <= alen; i++) {
    for (j = 1; j <= blen; j++) {

      int scost = (a[i - 1] == b[j - 1]) ? 0 : 1;

      matrix[i][j] = min3(matrix[i - 1][j] + 1,
                          matrix[i][j - 1] + 1,
                          matrix[i - 1][j - 1] + scost);
    }
  }

  res = matrix[alen][blen];

  for (i = 0; i <= alen; i++)
    free(matrix[i]);
  free(matrix);

  return res;
}

PG_FUNCTION_INFO_V1(lev_v1);

Datum
lev_v1(PG_FUNCTION_ARGS) {
  char *a, *b;
  float8 res;

  a = DatumGetPointer(DirectFunctionCall1(textout,
                                          PointerGetDatum(PG_GETARG_TEXT_P(0))));
  b = DatumGetPointer(DirectFunctionCall1(textout,
                                          PointerGetDatum(PG_GETARG_TEXT_P(1))));

  res = (float8) _lev_v1(a, b);

  PG_RETURN_FLOAT8(res);
}
