#include "similarity.h"

typedef struct lev_t {
  text *x, *y;
} lev_t;


PG_FUNCTION_INFO_V1(lev_t_in);
Datum
lev_t_in(PG_FUNCTION_ARGS) {
  lev_t *result;
  result = (lev_t *) palloc(sizeof(lev_t));
  result->x = PG_GETARG_TEXT_PP(0);
  result->y = PG_GETARG_TEXT_PP(1);
  PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(lev_t_less);
Datum
lev_t_less(PG_FUNCTION_ARGS) {
  lev_t *l = (lev_t *) PG_GETARG_POINTER(0);
  int max_d = PG_GETARG_INT32(1);
  text *src = l->x;
  text *dst = l->y;
  const char *s_data, *t_data;
  int s_bytes, t_bytes;
  s_data = VARDATA_ANY(src);
  t_data = VARDATA_ANY(dst);

  s_bytes = VARSIZE_ANY_EXHDR(src);
  t_bytes = VARSIZE_ANY_EXHDR(dst);
  PG_RETURN_BOOL(_lev_v5(s_data, s_bytes, t_data, t_bytes, max_d));
}
