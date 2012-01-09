#include "libdata.h"

Map* Map_new (size_t size, compare_pred comp)
{
  mPool* mpool = mPool_new(size * sizeof(Pair));
  Map* map = MPOOL_ALLOC(mpool, Map);
  Array* array = Array_new(size);

  map->mpool = mpool;
  map->comp  = comp;
  map->data  = array;

  return map;
}

void Map_free (Map *map)
{
  Array_free(map->data);
  mPool_free(map->mpool);
}

void Map_put (Map *map, P key, P val)
{
  Pair *pair = Map_search(map, key);

  if (pair)
    pair->cdr = val;
  else {
    pair = MPOOL_ALLOC(map->mpool, Pair);
    pair->car = key;
    pair->cdr = val;
    Array_append(map->data, pair);
  }
}

P Map_ref (Map *map, P key)
{
  Pair *pair = Map_search(map, key);
  return pair ? pair->cdr : NULL;
}

Pair* Map_search (Map *map, P key)
{
  register int i;
  Array *arr = map->data;
  
  Array_foreach(i, arr) {
    if (map->comp(key, ((Pair*)Array_ref(arr, i))->car) == 0)
      return (Pair*)Array_ref(arr, i);
  }
  return NULL;
}

#ifdef _TEST
int main ()
{
  Map *map = Map_new(5, COMPARE_PRED(strcmp));

  Map_put(map, "abc", "ABC");
  TEST(!strcmp(Map_ref(map, "abc"), "ABC"));

  TEST(MAP_SIZE(map) == 1);

  TEST(!Map_ref(map, "sskskssk"));

  Map_put(map, "abc", "aaaa");
  TEST(!strcmp(Map_ref(map, "abc"), "aaaa"));
  TEST(MAP_SIZE(map) == 1);

  return 0;
}
#endif
