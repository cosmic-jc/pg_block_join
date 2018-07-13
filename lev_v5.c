#include "similarity.h"

bool _lev_v5(const char *source, int m, const char *target, int n, int max_d) {
  int *prev, *curr;
  int i, j, zp;
  const char *y;
  int start_column, stop_column;
  int min_theo_d, inc;

  if (max_d <= 0) return false;

  if (!m) return (n < max_d);
  if (!n) return (m < max_d);
  inc = n - m;

  min_theo_d = Abs(inc);

  if (max_d < min_theo_d)
    return false;

  if (max_d > Max(m, n))
    return true;

  start_column = 0;
  stop_column = ((inc < 0) ? -inc : 0) + ((max_d - min_theo_d - 1) >> 1) + 1;
  if (stop_column > m)
    stop_column = m + 1;

  ++m;
  ++n;

  prev = (int *) palloc(2 * m * sizeof(int));
  curr = prev + m;

  for (i = start_column; i < stop_column; i++)
    prev[i] = i;

  for (y = target, j = 1; j < n; j++) {
    int *temp;
    const char *x = source;

    if (stop_column < m) {
      prev[stop_column] = max_d;
      ++stop_column;
    }

    if (start_column == 0) {
      curr[0] = j;
      i = 1;
    } else
      i = start_column;

    for (; i < stop_column; i++) {
      int c1, c2, c3;
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
    zp = j - (n - m);
    while (stop_column > 0) {
      int ii = stop_column - 1;
      int inc = ii - zp;
      if (prev[ii] + Abs(inc) < max_d)
        break;
      stop_column--;
    }
    while (start_column < stop_column) {
      int inc = start_column - zp;
      if (prev[start_column] + Abs(inc) < max_d)
        break;
      prev[start_column] = max_d;
      curr[start_column] = max_d;
      if (start_column != 0)
        source += 1;
      start_column++;
    }

    if (start_column >= stop_column)
      return false;
  }
  return (prev[m - 1] < max_d);
}

PG_FUNCTION_INFO_V1(lev_v5);
Datum
lev_v5(PG_FUNCTION_ARGS) {
  text *src = PG_GETARG_TEXT_PP(0);
  text *dst = PG_GETARG_TEXT_PP(1);
  int max_d = PG_GETARG_INT32(2);
  const char *s_data, *t_data;
  int s_bytes, t_bytes;

  s_data = VARDATA_ANY(src);
  t_data = VARDATA_ANY(dst);

  s_bytes = VARSIZE_ANY_EXHDR(src);
  t_bytes = VARSIZE_ANY_EXHDR(dst);

  PG_RETURN_BOOL(_lev_v5(s_data, s_bytes, t_data, t_bytes, max_d));
}
