// main.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

#include "type.h"

struct Type.Int type03;

int main(int argc, char *argv[]) {
  printf("Start\n");
  InitTypeInt(&type03);
  DebugOutOfType<(Type*)&type03>(stdout);

  struct Type.Int type01;
  InitTypeInt(&type01);
  DebugOutOfType<(Type*)&type01>(stdout);

  struct Type* type02 = CreateTypeInt();
  DebugOutOfType<type02>(stdout);
  free(type02);

  printf("Stop\n");
  exit(EXIT_SUCCESS);
}
