#include "libdata.h"

extern void Pair_free_rec(Pair *pair)
{
  Pair *next;
  for (;;) {
    if (!CDR(pair)) break;
    next = CDR(pair);
    free(pair);
    pair = next;
  }
}

extern inline Pair *Pair_set(Pair *pair, P car, P cdr)
{
  PAIR_SETCAR(pair, car);
  PAIR_SETCDR(pair, cdr);
  return pair;
}

void Pair_append (Pair *pair, P p)
{
  while (CDR(pair))
    pair = CDR(pair);
  
  PAIR_SETCDR(pair, p);
}

int Pair_length (Pair *pair)
{
  register int len = 1;
  for (;;) {
    if (!CDR(pair)) break;
    pair = CDR(pair);
    len++;
  }
  return len;
}

Pair *Pair_nth (Pair *pair, unsigned int i)
{
  while (i-- && (pair = CDR(pair)));
  return pair;
}

Pair *Pair_last (Pair *pair)
{
  while (CDR(pair)) pair = CDR(pair);
  return pair;
}

Pair *Pair_reverse (Pair *pair)
{
  Pair *first, *next, *result = NULL;
  
  if (!CDR(pair))
    return pair;
  
  for (first = pair; first; first = next) {
    next = CDR(first);
    PAIR_SETCDR(first, result);
    result = first;
  }
  
  return result;
}

Pair *Pair_memq (P p, Pair *pair)
{
  PAIR_FOREACH(pair, pair)
    if (p == CAR(pair))
      return pair;
  return NULL;
}

Pair *Pair_mems (char *s, Pair *pair)
{
  PAIR_FOREACH(pair, pair)
    if (EQS(s, (char*)CAR(pair)))
      return pair;
  return NULL;
}

#ifdef _TEST
int main ()
{
  Pair *pair = Pair_set(NEW(Pair), (P)0xf0f0f0, NULL);

  TEST(CAR(pair) == (P)0xf0f0f0);
  TEST(CDR(pair) == NULL);

  pair->cdr = Pair_set(NEW(Pair), (P)0xfff, NULL);

  TEST(CAR(CDR(pair)) == (P)0xfff);
  
  TEST(Pair_length(pair) == 2);
  
  Pair_append(pair, Pair_set(NEW(Pair), (P)0xeee, NULL));
  TEST(CAR(CDR(CDR(pair))) == (P)0xeee);
  TEST(Pair_length(pair) == 3);
  TEST(CAR(Pair_last(pair)) == (P)0xeee);
  
  TEST(CAR(Pair_nth(pair, 0)) == (P)0xf0f0f0);
  TEST(CAR(Pair_nth(pair, 1)) == (P)0xfff);
  TEST(CAR(Pair_nth(pair, 2)) == (P)0xeee);
  TEST(Pair_nth(pair, 3) == NULL);

  pair = Pair_reverse(pair);
  TEST(CAR(pair) == (P)0xeee);
  TEST(CAR(Pair_last(pair)) == (P)0xf0f0f0);

/*   TIME(DOTIMES(i, 10, */
/*                TIMEC(1000000, pair = Pair_set(NEW(Pair), NULL, P(pair))); */
/*                Pair_free_rec(pair))); */

  return 0;
}
#endif
