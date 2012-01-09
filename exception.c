#include "libdata.h"

static Stack __ExceptionTop_stack__ = { NULL, 0, 0 };
static Exception __ExceptionTop__ = { NULL, NULL, 0, 0, &__ExceptionTop_stack__ };
Exception *ExceptionTop = &__ExceptionTop__;

Exception *Exception_new ()
{
  Exception *exc = NEW(Exception);
  exc->file = (char*)NULL;
  exc->mess = (char*)NULL;
  exc->line = 0;
  exc->ret  = 0;
  exc->sta  = Stack_new(1);
  return exc;
}

void Exception_free (Exception *exc)
{
  Stack_free(exc->sta);
  free(exc);
}

void __throw__ (Exception *exc, int ret, const char *mess, const char *file, unsigned int line)
{
  if (exc)
    if (Stack_empty(exc->sta))
      __throw__(NULL, ret, mess, file, line);
    else {
      exc->line = line;
      exc->file = file;
      exc->mess = mess;
      exc->ret  = ret;
      longjmp(*(jmp_buf*)(Stack_pop(exc->sta)), ret);
    }
  else if (Stack_empty(ExceptionTop->sta)) {
    fprintf(stderr, "%s - %d: exception stack empty\n", file, line);
    exit(1);
  } else
    __throw__(ExceptionTop, ret, mess, file, line);
}

#ifdef _TEST
#include <string.h>

void a (Exception *exc) {
  throw(exc, 123, NULL);
}

void b (Exception *exc) {
  with_exception(exc, throw(exc, exc->ret, NULL), throw(exc, Exception_level(exc), NULL));
}

void c (Exception *exc)
{
  return;
}

int main ()
{
  Exception *exc = Exception_new();
  
  with_exception(exc, TEST(exc->ret == 20), throw(exc, 20, NULL));

  with_exception(exc, TEST(!strcmp(exc->mess, "HOGE")), throw(exc, 888, "HOGE"));

  with_exception(exc, TEST(exc->ret == 1), throw(exc, (Exception_level(exc)), NULL));
  TEST(Exception_level(exc) == 0);

  with_exception(exc, TEST(exc->ret == 123), a(exc));

  with_exception(exc, TEST(exc->ret == 2), b(exc));
  TEST(Exception_level(exc) == 0);

  with_exception(exc, , c(exc));
  TEST(Exception_level(exc) == 0);

  with_exception(ExceptionTop, TEST(ExceptionTop->ret == 33388), THROW(33388, NULL));

  return 0;
}

#endif
