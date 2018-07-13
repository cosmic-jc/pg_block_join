#include "similarity.h"

#define GET_CHAR(x,y) ((int)toupper(VARDATA((x))[(y)]))
#define SHIFT(x,y) (((x)%128) * 128 + (y))

PG_FUNCTION_INFO_V1(jac);

Datum
jac(PG_FUNCTION_ARGS) {
  static int flag[65536];
  text *t1 = (text *)PG_GETARG_TEXT_P(0);
  text *t2 = (text *)PG_GETARG_TEXT_P(1);

  int m = VARSIZE(t1) - VARHDRSZ, n = VARSIZE(t2) - VARHDRSZ;
  int cnt = 0, cnt_s = 0, i,sign;
  float4 result = .0;


  if(m && n) {
    sign = 0;
    for(i = 0; i < m; i++) {
      sign = SHIFT(sign, GET_CHAR(t1,i));
      if(!flag[sign]) {
        flag[sign] = 1;
        cnt++;
      }
    }
    sign = SHIFT(sign, 0);
    flag[sign] = 1;
    cnt++;

    sign = 0;
    for(i = 0; i < n; i++) {
      sign = SHIFT(sign, GET_CHAR(t2,i));
      if(!flag[sign]) {
        cnt++;
      } else if(flag[sign] == 1) {
        cnt_s++;
      }
      flag[sign] = 2;
    }
    sign = SHIFT(sign, 0);
    if(flag[sign] == 1)
      cnt_s++;
    else
      cnt++;

    result = (float4)cnt_s / cnt;
    memset(flag,0, sizeof(flag));
  }

  PG_RETURN_FLOAT4(result);
}
