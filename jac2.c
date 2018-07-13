#include "similarity.h"

#define SHIFT(x,y) (((x)<<8)|(y))

PG_FUNCTION_INFO_V1(jac2);

Datum
jac2(PG_FUNCTION_ARGS) {
  text *src = PG_GETARG_TEXT_PP(0);
  text *dst = PG_GETARG_TEXT_PP(1);
  const char *s_data, *t_data;
  int s_bytes, t_bytes;

  s_data = VARDATA_ANY(src);
  t_data = VARDATA_ANY(dst);

  s_bytes = VARSIZE_ANY_EXHDR(src);
  t_bytes = VARSIZE_ANY_EXHDR(dst);

  PG_RETURN_FLOAT4(_jac2(s_data, s_bytes, t_data, t_bytes));
}

float4 _jac2(const char *s, int m, const char *t, int n) {
  static int flag[65536];
  int clean_list[200];

  int cnt = 0, cnt_s = 0, i;
  short sign;
  float4 result = .0;

  /*float target;*/
  /*target = PG_GETARG_FLOAT4(2);*/
  /*if((float)(m + 1) / (n + 1) < target)*/
  /*return 0;*/
  /*if((float)(n + 1) / (m + 1) < target)*/
  /*return 0;*/


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

  return result;
}
