#include "libdata.h"

String *String_new (size_t size)
{
  if (size <= 1) return NULL;
  
  String *str = NEW(String);
  str->ptr = ALLOC(size);
  str->size = size;
  str->used = 0;

  return str;
}

void String_free (String *s)
{
  free(s->ptr);
  free(s);
}

static void String_resize (String *buf, size_t s)
{
  buf->size += buf->size * 2 + s;
  buf->ptr = realloc(buf->ptr, sizeof(*buf->ptr) * buf->size);
  if (buf->ptr == NULL)
    THROW(ENOMEM, "Strinb->ptr");
}

#define STRING_FILLP(str, len) (((str)->used + len) > (str)->size)
#define STRING_RESIZE(str, len) if (STRING_FILLP(str, len)) String_resize(str, len);

char *String_get (String *buf)
{
  char *x = (char *)(buf->ptr + buf->used);
  *x = '\0';
  return buf->ptr;
}

inline void String_append_size (String *buf, const char *str, size_t len)
{
  STRING_RESIZE(buf, len);
  memcpy(buf->ptr + buf->used, str, len);
  buf->used += len;
}

inline void String_append (String *buf, const char *str)
{
  String_append_size(buf, str, strlen(str));
}

inline void String_appendc (String *str, char c)
{
  STRING_RESIZE(str, 1);
  char *cb = (char*)(str->ptr + str->used); *cb = c;
  str->used++;
}

static const char __xdigs[16] = "0123456789ABCDEF";

void String_appendh (String *str, unsigned char c)
{
  String_appendc(str, __xdigs[(c >> 4) & 15]);
  String_appendc(str, __xdigs[c & 15]);
}

void String_snprintfa (String *str, size_t size, const char *fmt, ...)
{
  char buf[size+1];
  va_list params;
  
  va_start(params, fmt);
  if (vsnprintf(buf, size, fmt, params) < 0) THROW(USER_ERROR, "String_snprintfa");
  va_end(params);

  String_append(str, buf);
}

int String_fprint (String *s, FILE *fp)
{
  return (fputs(s->ptr, fp));
}

char *string_trim (char *str)
{
  int len = strlen(str);
  for (len--; str[len] == ' ' || str[len] == '\n' || str[len] == '\t'; len--)
    str[len] = '\0';

  int pos = 0;
  while (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\t')
    pos++;

  return &str[pos];
}

inline void uchar2hex (unsigned char c, unsigned char r[2])
{
  r[0] = __xdigs[(c >> 4) & 15];
  r[1] = __xdigs[c & 15];
}

bool string_null (char *str)
{
  return STRING_NULL(str);
}

bool s_isalnum (char *str)
{
  if (STRING_NULL(str))
    return false;
  else {
    char c;
    while ((c = *str++))
      if (!isalnum(c)) return false;
    return true;
  }
}

bool s_isalpha (char *str)
{
  if (STRING_NULL(str))
    return false;
  else {
    char c;
    while ((c = *str++))
      if (!isalpha(c)) return false;
    return true;
  }
}

bool s_isdigit (char *str)
{
  if (STRING_NULL(str))
    return false;
  else {
    char c;
    while ((c = *str++))
      if (!isdigit(c)) return false;
    return true;
  }
}

bool s_isdigit_negative (char *snum)
{
  return S_ISDIGIT_NEGATIVE(snum);
}

unsigned char _hex_table[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
  0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x0B, 0x0C,
  0x0D, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x0B, 0x0C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00
};

#ifdef _TEST
int main ()
{
  String *str;
  
  str = String_new(1000);

  String_append(str, "hoge");
  TEST(!strcmp(String_get(str), "hoge"));
  
  String_append(str, "DEF");
  TEST(!strcmp(String_get(str), "hogeDEF"));

  String_appendc(str, '!');
  TEST(!strcmp(String_get(str), "hogeDEF!"));

  String_free(str);

  str = String_new(10);
  String_appendh(str, 'z');
  TEST(*String_get(str) == '7' && *(String_get(str) + 1) == 'A');
  String_free(str);
  
  str = String_new(10);
  int i = 100000;
  while (i--) String_append(str, "abc");
  TEST(300000 == (int)str->used);
  String_free(str);

  str = String_new(3);
  String_snprintfa(str, 5, "%d", 8000);
  TEST(!strcmp(String_get(str), "8000"));
  String_free(str);

  str = String_new(100);
  String_snprintfa(str, 5, "%d", 28);
  TEST(!strcmp(String_get(str), "28"));
  
  String_snprintfa(str, 20, " %d %d", 88, 22);
  TEST(!strcmp(String_get(str), "28 88 22"));
  String_free(str);

  char *xxx = strdup(" \tabc \n");
  TEST(!strcmp(string_trim(xxx), "abc"));
  TEST(!strcmp(xxx, " \tabc"));

  TEST(string_null(""));
  TEST(string_null(NULL));
  TEST(!string_null("kk"));
  
  TEST(s_isalnum("abcde8"));
  TEST(!s_isalnum("abc;de8"));
  TEST(!s_isalnum(NULL));
  TEST(!s_isalnum(""));
  
  TEST(!s_isalpha("abc;de8"));
  TEST(s_isalpha("abcddddd"));
  TEST(!s_isalpha(NULL));
  TEST(!s_isalpha(""));
  
  TEST(!s_isdigit("abcddddd"));
  TEST(s_isdigit("33333"));
  TEST(!s_isdigit(NULL));
  TEST(!s_isdigit(""));

  TEST(!s_isdigit_negative("22222"));
  TEST(s_isdigit_negative("-22222"));
  TEST(!s_isdigit_negative("-22a222"));

#ifdef DEBUG
  int i = 10000000;
  puts("Allocate 1");
  while (i--)
    String_appendc(str, 'a');
  sleep(3);
  puts("Free 1");
  String_free(str);
  sleep(3);

  i = 10000000;
  str = String_new(5);
  puts("Allocate 2");
  while (i--)
    String_append(str, "AAA");
  sleep(3);
  puts("Free 2");
  String_free(str);

  i = 10000000;
  str = String_new(5);
  puts("Allocate 3");
  while (i--)
    String_snprintfa(str, 20, "%d", i);
  sleep(3);
  puts("Free 3");
  String_free(str);

  sleep(3);
#endif // DEBUG

  return 0;
}

#endif
