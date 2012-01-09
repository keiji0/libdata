#include "libdata.h"

Array *Array_new(size_t size){
  if(size < 1) return NULL;
  
  Array *a = NEW(Array);
  a->size = size;
  a->used = 0;
  a->data = (P*)ALLOC(size * sizeof(P));

  return a;
}

P Array_ref(Array *a, int index){
  return ARRAY_REF(a, index);
}

void Array_free (Array *a)
{
  if (!a) return;
  free(a->data);
  free(a);
}

void Array_clean (Array *a)
{
  register size_t i;
  for (i = 0; i < Array_length(a); i++)
    a->data[i] = NULL;
}

void Array_append (Array *a, P p)
{
  if (a->used >= a->size) {
    a->size += a->size + 2;
    a->data = realloc(a->data, sizeof(*a->data) * a->size);
    if (a->data == NULL) THROW(ENOMEM, "Array_append");
  }
  a->data[a->used++] = p;
}

P Array_pop (Array *a)
{
  return (Array_empty(a)) ? NULL : (a->data[--a->used]);
}

#ifdef _TEST
int main ()
{
  Array *a = Array_new(1);

  Array_append(a, "hoge");
  TEST(!strcmp(Array_ref(a, 0), "hoge"));
  TEST(Array_ref(a, 1) == NULL);
  TEST(a->used == 1);
  
  Array_append(a, "bar");
  TEST(!strcmp(Array_ref(a, 1), "bar"));
  TEST(a->used == 2);
  
  Array_append(a, "jojo");
  TEST(!strcmp(Array_ref(a, 2), "jojo"));
  TEST(a->used == 3);

  char *x = Array_pop(a);
  TEST(!strcmp(x, "jojo"));
  x = Array_pop(a);
  TEST(!strcmp(x, "bar"));
  TEST(!strcmp(Array_pop(a), "hoge"));
  Array_append(a, "TEST");
  TEST(!strcmp(Array_pop(a), "TEST"));
  TEST(!Array_pop(a));

  Array_clean(a);
  TEST(!Array_ref(a, 0));
  TEST(!Array_ref(a, Array_used(a)));

  Array A = { NULL, 0, 0 };
  Array_append(&A, "sss");
  TEST(!strcmp(Array_ref(&A, 0), "sss"));

#ifdef DEBUG
  int i = 1000000;
  while (i--)
    Array_append(a, "abcdefg");
  printf("used = %d, size = %d\n", a->used, a->size);
  sleep(3);

  i = 1000000;
  while (i--)
    Array_append(a, "abcdefg");
  printf("used = %d, size = %d\n", a->used, a->size);
  sleep(3);

  i = 1000000;
  while (i--)
    Array_append(a, "abcdefg");
  printf("used = %d, size = %d\n", a->used, a->size);
  sleep(3);

  Array_free(a);
  sleep(3);
#endif // DEBUG
  return 0;
}
#endif // _TEST
