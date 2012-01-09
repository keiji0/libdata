#include "length.h"

extern unsigned int string_length(register char *s)
{
  register char *t;

  t = s;
  for (;;) {
    if (!*t) return t - s; ++t;
    if (!*t) return t - s; ++t;
    if (!*t) return t - s; ++t;
    if (!*t) return t - s; ++t;
  }
}
