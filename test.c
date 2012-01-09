#include "libdata.h"

void __pt_times__ (clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
  static long clk = 0;

  if (clk == 0)
    if ((clk = sysconf(_SC_CLK_TCK)) < 0)
      err_sys("sysconf error");

  fprintf(stderr, "real: %f, ", real / (double)clk);
  fprintf(stderr, "user: %f, ", (tmsend->tms_utime - tmsstart->tms_utime) / (double)clk);
  fprintf(stderr, "sys: %f\n", (tmsend->tms_stime - tmsstart->tms_stime) / (double)clk);
}

#ifdef _TEST
int main () {
  
  return 0;
}
#endif
