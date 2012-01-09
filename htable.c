#include "libdata.h"

static size_t str_hash (const P p);
static bool str_comp (const P key1, const P key2);

static size_t prime_number (size_t x)
{
#define X(p) if (x < p) return p
  X(103);
  X(229);
  X(467);
  X(977);
  X(1979);
  X(4019);
  X(6037);
  X(7951);
  X(12149);
  X(16231);
  X(33493);
  return 65357;
#undef X
}

HT *HT_new(size_t length, HT_hash hf, HT_comp hc)
{
  length = prime_number(length ? length : 1979);
  
  mPool *mpool = mPool_new(HASHTABLE_COMPUTE_CHUNK_SIZE(length));
  HT *ht = (HT*)mPool_alloc(mpool, sizeof(HT));
  HashNode **nodes = (HashNode**)malloc(length * sizeof(HashNode*));

  ht->mpool = mpool;
  ht->nodes = nodes;
  ht->length = length;
  ht->count = 0;
  ht->hash = hf ? hf : str_hash;
  ht->comp = hc ? hc : str_comp;

  return ht;
}

void HT_free (HT *ht)
{
  free(ht->nodes);
  mPool_free(ht->mpool);
}

/* static void HT_resize (HT *ht) */
/* { */
/*   size_t i, ld_size = ht->size; ht->size *= 2; */
/*   HashNode *node, */
/*            **old_nodes = ht->nodes, */
/*            **new_nodes = (HashNode**)malloc(chunk, sizeof(HashNode*)); */

/*   for (i = 0; i <= old_size; i++) { */
/*     if ((node = old_nodes[i])) { */
/*       do { */
/*         new_nodes[HT_index(ht, node->key)] = node; */
/*       } while ((node = node->next)); */
/*     } */
/*   } */
/*   free(old_nodes); */
/* } */

static HashNode *HT_nextNode (HT *ht, HashNode *node, const P key)
{
  if (!node)
    return NULL;
  else {
    while (node)
      if (ht->comp(key, node->key))
        return node;
      else
        node = node->next;
    return NULL;
  }
}

static HashNode *HashNode_new (HT *ht, const P key, const P val)
{
  HashNode *node = (HashNode*)(mPool_alloc(ht->mpool, sizeof(HashNode)));
  node->key = key;
  node->val = val;
  node->next = NULL;
  ht->count++;
  return node;
}

P HT_ref (HT *ht, const P key)
{
  HashNode *node = HT_nextNode(ht, HT_findNode(ht, key), key);
  return node ? node->val : NULL;
}

void HT_put (HT *ht, const P key, const P val)
{
  size_t index = HT_index(ht, key);
  HashNode *node  = ht->nodes[index], *nn;

  if (node) {
    if ((nn = HT_nextNode(ht, node, key))) {
      nn->val = val;
    } else {
      HashNode *new = HashNode_new(ht, key, val);
      ht->nodes[index] = new; new->next = node;
    }
  } else {
    node = HashNode_new(ht, key, val);
    ht->nodes[index] = node;
  }
}

void HT_add (HT *ht, const P key, const P val)
{
  size_t index = HT_index(ht, key);
  HashNode *node = ht->nodes[index];

  if (node) {
    HashNode *new = HashNode_new(ht, key, val);
    ht->nodes[index] = new; new->next = node;
  } else {
    node = HashNode_new(ht, key, val);
    ht->nodes[index] = node;
  }
}

bool HT_delete (HT *ht, const P key)
{
  size_t index = HT_index(ht, key);
  HashNode *node = ht->nodes[index];

  if (!node) return false;
  ht->nodes[index] = node->next;
  ht->count--;
  return true;
}

static size_t str_hash (const P p)
{
  const char *key = (char*)p;
  unsigned char c;
  unsigned int hv = 0;
  while ((c = *key++)) hv = (hv << 5) - hv + c;
  return hv;
}

static bool str_comp (const P key1, const P key2)
{
  return EQS(key1, key2);
}

#ifdef _TEST
int main ()
{
  HT *ht = HT_new(50000, NULL, NULL);

  HT_put(ht, "abc", "sss");
  TEST(!strcmp("sss", HT_ref(ht, "abc")));
  
  HT_add(ht, "abc", "hoge");
  TEST(!strcmp("hoge", HT_ref(ht, "abc")));

  HT_delete(ht, "abc");
  TEST(!strcmp("sss", HT_ref(ht, "abc")));
  
  return 0;
}
#endif // _TEST
