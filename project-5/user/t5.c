// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N  1000

void foo1(const char *s) { write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
  write(1, s, strlen(s)); write(1, s, strlen(s)); } void foo2(const char *s) {
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
    write(1, s, strlen(s)); } void foo3(const char *s) { write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
      write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s)); }
      void foo4(const char *s) { write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
        write(1, s, strlen(s)); write(1, s, strlen(s)); } void foo5(const char
            *s) { write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
              strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); write(1, s, strlen(s));
            write(1, s, strlen(s)); write(1, s, strlen(s)); write(1, s,
                strlen(s)); write(1, s, strlen(s)); }

int
main(void)
{
  printf("before fork\n");
  sleep(10);

  int a = fork();
  int b = fork();
  int c = fork();

  if (a && b && c) {
    printf("forked (%p)\n", ((uint64)&foo1)|((uint64)&foo2)|((uint64)&foo3)|((uint64)&foo4)|((uint64)&foo5));
  }
  sleep(10);
  exit(0);
}
