// main.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

// #include "context.h"

// struct Constant.Int intConstExt;
// struct Context<constant<Int> > intConstContextExt;

// Начальные установки параметров компилятора и его запуск
_Bool StartCompiler(wchar_t *in, size_t in_len);

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, ""); //get the OS's locale.
  printf("Start\n");

  struct stat sb;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if (lstat(argv[1], &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }

  printf("ID of containing device:  [%x,%x]\n",
          major(sb.st_dev),
          minor(sb.st_dev));

  printf("File type:                ");

  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
  }

  printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);
  printf("Mode:                     %jo (octal)\n",
          (uintmax_t) sb.st_mode);
  printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);
  printf("Ownership:                UID=%ju   GID=%ju\n",
          (uintmax_t) sb.st_uid, (uintmax_t) sb.st_gid);
  printf("Preferred I/O block size: %jd bytes\n",
          (intmax_t) sb.st_blksize);
  printf("File size:                %jd bytes\n",
          (intmax_t) sb.st_size);
  printf("Blocks allocated:         %jd\n",
          (intmax_t) sb.st_blocks);
  printf("Last status change:       %s", ctime(&sb.st_ctime));
  printf("Last file access:         %s", ctime(&sb.st_atime));
  printf("Last file modification:   %s", ctime(&sb.st_mtime));

  // Далее создаем строку соответствующей длины и читаем файл
  size_t fileSize = sizeof(char) * sb.st_size;
  char *unitBuffer = malloc(fileSize + 2);
  FILE *inFile = fopen(argv[1], "r");
  size_t stringLength = fread(unitBuffer, sizeof(char), fileSize, inFile);
  fclose(inFile);
  unitBuffer[stringLength] = '\0';

  // Тестовый вывод информации о прочитанном файле.
  printf("\nstringLength = %ld\n", stringLength);
  printf("%s", unitBuffer);

  // Формирование строки из широких символов (рун?) и заполнение ее
  wchar_t *unitBufferUtf32 = malloc(stringLength * 4);
  // Перенос текущей строки в строку рун
  size_t widthLengthUtf32 = mbstowcs(unitBufferUtf32, unitBuffer, stringLength);
  if(widthLengthUtf32 == -1) {
    printf("Incorrect moving of unitBuffer to unitBufferUtf32\n");
    exit(5);
  }
  // unitBufferUtf32[widthLengthUtf32+1] = L'\0';
  unitBufferUtf32[widthLengthUtf32] = L'\0';

  printf("\nwidthLengthUtf32 = %ld\n", widthLengthUtf32);
  // Вывод широкой строки на дисплей.
  printf("%ls\n", unitBufferUtf32);

  // Запуск компилятора
  StartCompiler(unitBufferUtf32, widthLengthUtf32);

  // Очистка выделенной памяти
  free(unitBuffer);
  free(unitBufferUtf32);

  printf("Finish\n");
  exit(EXIT_SUCCESS);
}
