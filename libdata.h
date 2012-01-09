#ifndef LIBDATA_H
#define LIBDATA_H

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned char B; // Byte
typedef void* P; // Pointer
typedef void* pointer;
typedef size_t S;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int(*compare_pred)(const P x, const P y);

#define COMPARE_PRED(fun) ((compare_pred)fun)
#define OR(x, y) ((x) ? (x) : (y))
#define CNULL (char)NULL
#define blen(b) (strlen((const char*)b))

//* Bool

typedef unsigned char bool;

#define false (bool)0
#define true  (bool)1
#define BOOL(x) ((x) ? true : false)

//* Memory
  
__inline__ P __xmalloc__
  (size_t size, const char *mes, const char *file, unsigned int line);

// メモリを得る
#define ALLOC(size)\
  __xmalloc__(size, NULL, __FILE__, __LINE__)

// 指定のオブジェクトを得る
#define NEW(typename)\
  (typename*)__xmalloc__(sizeof(typename), #typename, __FILE__, __LINE__)

//* Array

typedef struct
{
  P *data;
  size_t used;
  size_t size;
} Array;

Array *Array_new (size_t size);
void Array_free (Array *);
void Array_clean (Array *);
void Array_append (Array *, P p);
P Array_pop (Array *);
P Array_ref (Array *, int index);
#define Array_used(array) ((array)->used)
#define Array_length(array) ((array)->size)
#define Array_empty(array) (!Array_used(array))
#define Array_foreach(i, array) for (i = 0; i < Array_used(array); i++)
#define ARRAY_REF(array, index) (((array)->used > index) ? (array)->data[index] : NULL)

//* Stack

typedef Array Stack;
#define Stack_new Array_new
#define Stack_free Array_free
#define Stack_empty Array_empty
#define Stack_push Array_append
#define Stack_pop Array_pop
#define Stack_size Array_used

//* Chunk

struct Chunk {
  P freep;
  struct Chunk *next;
};

typedef struct Chunk Chunk;

#define CHUNK_BLOCK(chunk) \
  (P)((size_t)(chunk) + sizeof(Chunk))

#define CHUNK_USED_SIZE(chunk) \
  ((size_t)(chunk)->freep - ((size_t)(CHUNK_BLOCK(chunk))))

#define CHUNK_REST_SIZE(max, chunk) \
  (max - (CHUNK_USED_SIZE(chunk)))

#define CHUNK_INC(chunk, size) (chunk->freep += size)

__inline__ Chunk *Chunk_new (size_t size);
void Chunk_free (Chunk *chunk);

//* Memory pool

struct mPool {
  size_t size;
  struct Chunk *chunk;
};

typedef struct mPool mPool;

#define MPOOL_REST_SIZE(mpool) \
  (CHUNK_REST_SIZE((mpool)->size, (mpool)->chunk))

#define MPOOL_FULLP(mpool, __size__) \
  (MPOOL_REST_SIZE(mpool) <= __size__)

mPool *mPool_new (size_t size);
void mPool_free (mPool *pool);
__inline__ P mPool_alloc (mPool *pool, size_t size);
__inline__ P mPool_copy (mPool *pool, const P src, size_t len);
__inline__ char *mPool_sdup (mPool *pool, const char *str);
#define MPOOL_COPY(pool, src, len) (memcpy(mPool_alloc(pool, len), src, len))
#define MPOOL_ALLOC(mpool, type) ((type*)(mPool_alloc(mpool, sizeof(type))))

//* Pair

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

__inline__ Pair *Pair_set (Pair *pair, P car, P cdr);
void Pair_append (Pair *pair, P p);

//* String

typedef struct {
  char *ptr;
  size_t size;
  size_t used;
} String;

String *String_new (size_t size);
void String_free (String *);
#define String_print(str) (write(STDOUT_FILENO, (str)->ptr, (str)->used))
#define String_size(str) ((str)->used)
#define String_length(str) ((str)->used)
#define String_empty(str) (!BOOL(String_size(str)))
char *String_get (String *buf);

__inline__ void String_append_size (String *, const char *, size_t);
__inline__ void String_append (String *, const char *);
__inline__ void String_appendc (String *, char);
void String_appendh (String *str, unsigned char c);
int String_fprint (String *s, FILE *fp);
void String_snprintfa (String *str, size_t size, const char *fmt, ...);
__inline__ void uchar2hex (unsigned char c, unsigned char r[2]);

bool s_isalnum (char *str);
bool s_isalpha (char *str);
bool s_isdigit (char *str);
bool s_isdigit_negative (char *snum);
#define S_ISDIGIT_NEGATIVE(snum) ((*snum == '-') ? s_isdigit(snum + 1) : false)

#define STRING_NULL(str) (BOOL(str ? (*(char*)str == '\0') : true))
#define EQS(x, y) ((*(char*)x == *(char*)y) ? !strcmp((char*)x, (char*)y) : false)
extern unsigned char _hex_table[];
#define CHAR2HEX(x) (_hex_table[(B)x])
#define CCHAR2HEX(x, y) ((CHAR2HEX(x) << 4) + CHAR2HEX(y))
#define ISXXDIGIT(x, y) ((isxdigit((unsigned char)x)) && (isxdigit((unsigned char)y)))

//* IO
char *fgetl (char *s, int size, FILE *fp);

//* Map
typedef struct {
  mPool *mpool;
  Array *data;
  compare_pred comp;
} Map;

Map *Map_new (size_t size, compare_pred comp);
void Map_free (Map *map);
void Map_put (Map *map, P key, P val);
P Map_ref (Map *map, P key);
Pair *Map_search (Map *map, P key);
#define MAP_SIZE(map) Array_used((map)->data)

//* HT
typedef size_t(*HT_hash)(const P key);
typedef bool(*HT_comp)(const P key1, const P key2);

typedef struct HashNodeRec {
  P key;
  P val;
  struct HashNodeRec *next;
} HashNode;

typedef struct {
  mPool *mpool;
  HashNode **nodes;
  size_t length;
  size_t count;
  HT_hash hash;
  HT_comp comp;
} HT;

#define HASHTABLE_COMPUTE_CHUNK_SIZE(size) \
  (((size / 10) * sizeof(HashNode)) + sizeof(HT))

#define HT_index(ht, key) ((ht)->hash(key) % ((ht)->length))
#define HT_index_ref(ht, index) (HashNode*)(ht)->nodes[index]
#define HT_findNode(ht, key) (HT_index_ref(ht, HT_index(ht, key)))

#define HT_foreach(ht, _key, _val, body) {               \
    P _key, _val;                                         \
    register unsigned int i;                                    \
    HashNode *node;                                             \
    for (i = 0; i < (ht)->length; i++)                          \
      for (node = (ht)->nodes[i]; node; node = node->next) {    \
        _key = node->key; _val = node->val;                     \
        body;                                                   \
      }                                                         \
  }

HT *HT_new (size_t length, HT_hash hf, HT_comp hc);
void HT_free (HT *ht);
P HT_ref (HT *ht, const P key);
void HT_put (HT *ht, const P key, const P val);
void HT_add (HT *ht, const P key, const P val);
bool HT_delete (HT *ht, const P key);

//* Exception
#include <setjmp.h>
#include <stdio.h>
#include <errno.h>

typedef struct {
  const char *file;
  const char *mess;
  unsigned int line;
  int ret;
  Stack *sta;
} Exception;

extern Exception *ExceptionTop;

Exception *Exception_new ();
void Exception_free (Exception *exc);
void __throw__ (Exception *exc, int ret, const char *mess, const char *file, unsigned int line);
#define throw(exc, ret, mes) (__throw__(exc, ret, mes,  __FILE__, __LINE__))
#define THROW(ret, mes) (throw(ExceptionTop, ret, mes))
#define Exception_level(exc) (Stack_size((exc)->sta))

#define SYNTAX_ERROR  10000
#define PROGRAM_ERROR 10001
#define USER_ERROR    10002

#define with_exception(exc, handler, body)        \
  {                                               \
    jmp_buf __jmp__;                              \
                                                  \
    Stack_push((exc)->sta, &__jmp__);             \
    if (((exc)->ret = setjmp(__jmp__)) == 0) {    \
      body;                                       \
      Stack_pop((exc)->sta);                      \
    } else {                                      \
      handler;                                    \
    }                                             \
  }

//* Error
void err_doit (const char *file, unsigned int line, const char *fmt, ...);
#define err_sys(...) { (err_doit(__FILE__, __LINE__, __VA_ARGS__)); exit(1); }

//* Test
#include <sys/times.h>
#include <unistd.h>

#define TEST(x)                                                         \
  if (x) {                                                              \
    printf("%s - %2d, \"" #x "\" ==> ok\n", __FILE__, __LINE__);        \
  } else {                                                              \
    fprintf(stderr, "%s %2d, \"" #x "\" ==> ERROR\n", __FILE__, __LINE__); \
    exit(1);                                                            \
  }

#define TEST_SECTION(x) \
  puts("--- Section " x " start ---")

#define DOTIMES(c, max, body) \
  { register unsigned int c = 0;\
    for (;;) { if (c < max) { body; c++;} else break; } }

void __pt_times__ (clock_t real, struct tms *tmsstart, struct tms *tmsend);
  
#define TIME(body) {                                                \
    struct tms tmsstart, tmsend;                                    \
    clock_t start, end;                                             \
                                                                    \
    if ((start = times(&tmsstart)) == -1) err_sys("times error");   \
    body;                                                           \
    if ((end = times(&tmsend)) == -1) err_sys("times error");       \
    __pt_times__(end - start, &tmsstart, &tmsend);                  \
  }

#define TIMEC(c, body) \
  { TIME(DOTIMES(_, c, body); fprintf(stderr, "loop: %d count -> ", c);); }

#endif // LIBDATA_H
