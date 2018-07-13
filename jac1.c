#include "similarity.h"

#define SHIFT(x,y) (((x)<<8)|(y))

PG_FUNCTION_INFO_V1(jac1);

Datum
jac1(PG_FUNCTION_ARGS) {
  static int flag[65536];
  int clean_list[200];
  text *t1 = (text *)PG_GETARG_TEXT_P(0);
  text *t2 = (text *)PG_GETARG_TEXT_P(1);
  const char *s, *t;
  int m, n;
  int cnt = 0, cnt_s = 0, i;
  short sign;
  float4 result = .0;
  float target;

  s = VARDATA(t1);
  t = VARDATA(t2);
  m = VARSIZE(t1) - VARHDRSZ, n = VARSIZE(t2) - VARHDRSZ;
  target = PG_GETARG_FLOAT4(2);
  if((float)(m + 1) / (n + 1) < target)
    PG_RETURN_FLOAT4(0);
  if((float)(n + 1) / (m + 1) < target)
    PG_RETURN_FLOAT4(0);


  if(m && n) {
    sign = 0;
    for(i = 0; i < m; i++) {
      sign = SHIFT(sign, toupper(s[i]));
      if(!flag[sign]) {
        flag[sign] = 1;
        clean_list[cnt++] = sign;
      }
    }
    sign = SHIFT(sign, 0);
    flag[sign] = 1;
    clean_list[cnt++] = sign;

    sign = 0;
    for(i = 0; i < n; i++) {
      sign = SHIFT(sign, toupper(t[i]));
      if(!flag[sign]) {
        clean_list[cnt++] = sign;
      } else if(flag[sign] == 1) {
        cnt_s++;
      }
      flag[sign] = 2;
    }
    sign = SHIFT(sign, 0);
    if(flag[sign] == 1)
      cnt_s++;
    else
      clean_list[cnt++] = sign;

    result = (float4)cnt_s / cnt;
    for(i = 0; i < cnt; i++) {
      flag[clean_list[i]] = 0;
    }
  }

  PG_RETURN_FLOAT4(result);
}
