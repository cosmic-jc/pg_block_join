#include "similarity.h"

int _lev_v3(const char *source, int m, const char *target, int n) {
  int *prev, *curr;
  int i, j;
  const char *y;

  if (!m) return n;
  if (!n) return m;

  ++m;
  ++n;

  prev = (int *) palloc(2 * m * sizeof(int));
  curr = prev + m;

  for (i = 0; i < m; i++)
    prev[i] = i;

  for (y = target, j = 1; j < n; j++) {
    int *temp;
    const char *x = source;

    curr[0] = j;
    i = 1;
    for (; i < m; i++) {
      int	c1, c2, c3;
      c1 = prev[i] + 1;
      c2 = curr[i - 1] + 1;
      c3 = prev[i - 1] + (tolower(*x) != tolower(*y));
      c2 = Min(c1, c2);
      curr[i] = Min(c2, c3);
      x++;
    }

    temp = curr;
    curr = prev;
    prev = temp;

    y++;
  }

  return prev[m - 1];
}

PG_FUNCTION_INFO_V1(lev_v3);

Datum
lev_v3(PG_FUNCTION_ARGS) {
  text *src = PG_GETARG_TEXT_PP(0);
  text *dst = PG_GETARG_TEXT_PP(1);
  const char *s_data, *t_data;
  int s_bytes, t_bytes;

  s_data = VARDATA_ANY(src);
  t_data = VARDATA_ANY(dst);

  s_bytes = VARSIZE_ANY_EXHDR(src);
  t_bytes = VARSIZE_ANY_EXHDR(dst);

  PG_RETURN_INT32(_lev_v3(s_data, s_bytes, t_data, t_bytes));
}
