#ifndef PAIR_TYPE_H
#define PAIR_TYPE_H

struct Pair {
  P car;
  P cdr;
};

typedef struct Pair Pair;

#define CAR(pair) ((pair)->car)
#define CDR(pair) ((Pair*)(pair)->cdr)
#define PAIR_CAR(pair, type) (type)((pair)->car)
#define Pair_car(pair) PAIR_CAR(pair, P)
#define PAIR_CDR(pair, type) (type)((pair)->cdr)
#define Pair_cdr(pair) PAIR_CDR(pair, Pair*)
#define PAIR_SETCAR(pair, x) ((pair)->car = (P)x)
#define PAIR_SETCDR(pair, x) ((pair)->cdr = (P)x)
#define PAIR_FOREACH(p, pair) for(p = pair; p; p = CDR(p))

#endif
