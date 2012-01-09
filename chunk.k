#include "chunk.h"

#define CHUNK_BLOCK(chunk) \
  (P)((size_t)(chunk) + sizeof(Chunk))

#define CHUNK_USED_SIZE(chunk) \
  ((size_t)(chunk)->freep - ((size_t)(CHUNK_BLOCK(chunk))))

#define CHUNK_REST_SIZE(max, chunk) \
  (max - (CHUNK_USED_SIZE(chunk)))

#define CHUNK_INC(chunk, size) (chunk->freep += size)

inline Chunk *Chunk_new (size_t size)
{
  Chunk *chunk = (Chunk*)ALLOC(sizeof(Chunk) + size);

  chunk->freep = CHUNK_BLOCK(chunk);
  chunk->next = NULL;

  return chunk;
}

void Chunk_free (Chunk *chunk)
{
  Chunk *next;
  do {
    next = chunk->next;
    free(chunk);
  } while ((chunk = next));
}

#ifdef _TEST
int main ()
{
  Chunk *chunk = Chunk_new(100);

  TEST(chunk->freep == CHUNK_BLOCK(chunk));

  CHUNK_INC(chunk, 20);
  TEST(chunk->freep > CHUNK_BLOCK(chunk));
  TEST(CHUNK_USED_SIZE(chunk) == 20);
  TEST(CHUNK_REST_SIZE(100, chunk) == 80);
  
  return 0;
}
#endif
