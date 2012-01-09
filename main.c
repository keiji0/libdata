#include "libdata.h"

int main ()
{
  unsigned char x[16], xx[33];
  md5("ssss", x);
  md5_hex(x, xx);
  xx[33] = '\0';
  puts((char*)xx);
  return 0;
}
