#include "libdata.h"

char *fgetl (char *s, int size, FILE *fp)
{
  if ((fgets(s, size, fp)) == NULL)
    return NULL;
  else {
    s = strchr(s, '\n');
    if (s != NULL) *s = '\0';
    return s;
  }
}

#ifdef _TEST
int main () { return 0; }
#endif
