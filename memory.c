#include "libdata.h"

inline P __xmalloc__ (size_t size, const char *mes, const char *file, unsigned int line)
{
  P p = malloc(size);

  // メモリが一杯になればエラー処理
  if (p == NULL)
    __throw__(ExceptionTop, ENOMEM, mes, file, line);
  
  return p;
}

#ifdef _TEST
int main ()
{
  return 0;
}
#endif
