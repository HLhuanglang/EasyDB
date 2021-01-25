#include <cstdarg>
#include <cstdio>

int test_fun(const char* fmt, ...) {
  char str_tmp[100];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(str_tmp, 100, fmt, ap);
  va_end(ap);
  printf("%s", str_tmp);
  return 1;
}

int main() {
  test_fun("%s,%d,%s\n", "aaa", 123, "bbb");
  return 0;
}