#include "char.h"

extern unsigned int string_char(register char *s, int c)
{
  /* 文字が最初に表われる位置を返す */
  
  register char ch;
  register char *t;

  ch = c;
  t = s;
  for (;;) {
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
    if (!*t) break; if (*t == ch) break; ++t;
  }
  return t - s;
}
