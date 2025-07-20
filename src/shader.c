#include <glad/glad.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

static uint32_t compileShader(int32_t shaderType, const char *shaderSource);
static int32_t checkShaderCompileErrors(uint32_t shader);
static int32_t checkProgramLinkErrors(uint32_t program);

uint32_t generateShaderProgram(const char *vertexPath,
                               const char *fragmentPath) {
  char *vertexSource = readFile(vertexPath);
  if (vertexSource == NULL) {
    printf("Error reading vertex shader file: %s\n", vertexPath);
    return 0;
  }
  char *fragmentSource = readFile(fragmentPath);
  if (fragmentSource == NULL) {
    printf("Error reading fragment shader file: %s\n", fragmentPath);
    return 0;
  }

  /* printf("vertex:\n%s", vertexSource); */
  /* printf("Fragment:\n%s", fragmentSource); */

  unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
  free(vertexSource);

  unsigned int fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentSource);
  free(fragmentSource);

  uint32_t shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  if (!checkProgramLinkErrors(shaderProgram)) {
    return 0;
  }

  return shaderProgram;
}

static uint32_t compileShader(int32_t shaderType, const char *shaderSource) {
  uint32_t shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  if (!checkShaderCompileErrors(shader)) {
    return 0;
  }

  return shader;
}

static int32_t checkShaderCompileErrors(uint32_t shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    return 0;
  }
  return shader;
}

static int32_t checkProgramLinkErrors(uint32_t program) {
  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    printf("ERROR::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    return 0;
  }
  return program;
}
