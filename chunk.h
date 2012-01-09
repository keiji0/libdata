#ifndef CHUNK_H
#define CHUNK_H

struct Chunk {
  void *free;
  struct Chunk *next;
};

/* __inline__ struct Chunk make_chunk(size_t); */
/* void free_chunk(struct chunk); */

#endif
