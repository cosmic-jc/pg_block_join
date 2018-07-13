#include "similarity.h"

int _lev_v4(const char *source, int m, const char *target, int n, int max_d) {
  int *prev, *curr;
  int i, j;
  const char *y;
  int start_column, stop_column;

  if (!m) return n;
  if (!n) return m;

  start_column = 0;
  stop_column = m + 1;

  if (max_d > 0) {
    int min_theo_d;
    int max_theo_d;
    int net_inserts = n - m;

    min_theo_d = Abs(net_inserts);
    if (min_theo_d >= max_d)
      return max_d;
    max_theo_d = Max(m, n);
    if (max_d > max_theo_d)
      max_d = 0;
    else {
      int slack_d = max_d - min_theo_d - 1;
      int best_column = net_inserts < 0 ? -net_inserts : 0;

      stop_column = best_column + (slack_d / 2) + 1;
      if (stop_column > m)
        stop_column = m + 1;
    }
  }

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
    if (max_d > 0) {
      int zp = j - (n - m);
      while (stop_column > 0) {
        int ii = stop_column - 1;
        int net_inserts = ii - zp;
        if (prev[ii] + (net_inserts > 0 ? net_inserts : -net_inserts) < max_d)
          break;
        stop_column--;
      }
      while (start_column < stop_column) {
        int net_inserts = start_column - zp;
        if (prev[start_column] + (net_inserts > 0 ? net_inserts : -net_inserts) < max_d)
          break;
        prev[start_column] = max_d;
        curr[start_column] = max_d;
        if (start_column != 0)
          source += 1;
        start_column++;
      }
      if (start_column >= stop_column)
        return max_d;
    }
  }
  return prev[m - 1];
}

PG_FUNCTION_INFO_V1(lev_v4);
Datum
lev_v4(PG_FUNCTION_ARGS) {
  text *src = PG_GETARG_TEXT_PP(0);
  text *dst = PG_GETARG_TEXT_PP(1);
  int max_d = PG_GETARG_INT32(2);
  const char *s_data, *t_data;
  int s_bytes, t_bytes;

  s_data = VARDATA_ANY(src);
  t_data = VARDATA_ANY(dst);

  s_bytes = VARSIZE_ANY_EXHDR(src);
  t_bytes = VARSIZE_ANY_EXHDR(dst);

  PG_RETURN_INT32(_lev_v4(s_data, s_bytes, t_data, t_bytes, max_d));
}
