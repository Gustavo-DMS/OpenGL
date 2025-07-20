#ifndef SHADER_H
#define SHADER_H
#include <stdint.h>

uint32_t generateShaderProgram(const char *vertexShaderSource,
                               const char *fragmentShaderSource);
#endif // SHADER_H
