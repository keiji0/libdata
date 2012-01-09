#include "libdata.h"

void err_doit (const char *file, unsigned int line, const char *fmt, ...)
{
  va_list ap; va_start(ap, fmt);
  int errno_save = errno;

  if (fmt) {
    fprintf(stderr, "%s %d - %s -> ", file, line, strerror(errno_save));
    vfprintf(stderr, fmt, ap);
    fputs("\n", stderr);
  } else
    fprintf(stderr, "%s %d - %s\n", file, line, strerror(errno_save));

  va_end(ap);
}
