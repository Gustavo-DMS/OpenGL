#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readFile(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  uint32_t length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buf = malloc(sizeof(char) * (length + 1));
  fread(buf, 1, length, file);
  buf[length] = '\0';

  fclose(file);

  return buf;
}
#endif // UTILS_H
