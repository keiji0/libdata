#include "libdata.h"

mPool *mPool_new (size_t size)
{
  mPool *mpool = NEW(mPool);

  mpool->size  = size;
  mpool->chunk = Chunk_new(size);

  return mpool;
}

void mPool_free (mPool *mpool)
{
  Chunk_free(mpool->chunk);
  free(mpool);
}

inline P mPool_alloc (mPool *mpool, size_t size)
{
  if (mpool->size <= size) {
    Chunk *new = Chunk_new(size);
    new->next = mpool->chunk->next;
    mpool->chunk->next = new;
    return new->freep;
  } else {
    if (MPOOL_FULLP(mpool, size)) {
      Chunk *new = Chunk_new(mpool->size);
      new->next = mpool->chunk;
      mpool->chunk = new;
    }
    P p = mpool->chunk->freep;
    CHUNK_INC(mpool->chunk, size);
    return p;
  }
}

inline char *mPool_sdup (mPool *mpool, const char *src)
{
  size_t len = strlen(src);
  char *p = ((char*)MPOOL_COPY(mpool, src, len + 1));
  *(p + len + 1) = '\0';
  return p;
}

inline P mPool_copy (mPool *mpool, const P src, size_t len)
{
  return MPOOL_COPY(mpool, src, len);
}

#ifdef _TEST

void mPool_status (mPool *mpool)
{
  printf("mPool %lu byte (%p)\n", mpool->size, mpool);
  printf(" size:%5lu byte\n", sizeof(mPool));
  printf(" used:%5lu byte\n", mpool->size - MPOOL_REST_SIZE(mpool));
  printf(" rest:%5lu byte\n", MPOOL_REST_SIZE(mpool));
  printf(" chunk: %p -> %p\n", mpool->chunk, mpool->chunk->next);
  printf("   size:  %10lu byte\n", sizeof(Chunk));
  printf("   &next:  %10p\n", &mpool->chunk->next);
  printf("   &freep: %10p\n", &mpool->chunk->freep);
  printf("   start: %10p\n", (P)((size_t)mpool->chunk + sizeof(Chunk)));
  printf("   freep: %10p\n", mpool->chunk->freep);
  printf("   freep - start: %lu\n", (size_t)mpool->chunk->freep - ((size_t)mpool->chunk + sizeof(Chunk)));
}

int main ()
{
  mPool *mpool = mPool_new(256);
  Chunk *chunk = mpool->chunk;
  P x = chunk->freep;

  TEST(x == mPool_alloc(mpool, 5));
  TEST(((int)x + 5) == (int)mpool->chunk->freep);
  TEST(MPOOL_REST_SIZE(mpool) == 251);

  TEST(mPool_alloc(mpool, 0) == chunk->freep);
  
  mPool_alloc(mpool, 250);
  TEST(!mpool->chunk->next);

  TEST(!strcmp(mPool_sdup(mpool, "abc"), "abc"));

  mPool_free(mpool);

/*   int *ia; */
/*   DOTIMES(ii, 5, { */
/*       mpool = mPool_new(8192); */
/*       puts("mPool allocate"); */
/*       TIMEC(100000, { */
/*           ia = mPool_alloc(mpool, sizeof(int) * 100); */
/*           DOTIMES(i, 100, ia[i] = i); */
/*         }); */
/*       puts("mPool free"); */
/*       mPool_free(mpool); */
/*     }); */
  
  return 0;
}
#endif
