#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cglm/cglm.h>

#include "cglm/cam.h"
#include "cglm/mat4.h"
#include "stb_image.h"
#include "shader.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

float mixValue = 0.2f;
float xAxis = 0.0f;
float yAxis = 0.0f;
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.5f,
    0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, 0.5f,
    -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,
    1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  -0.5f,
    0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f,
    -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,
    1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  -0.5f,
    -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  1.0f,  0.5f,  -0.5f,
    0.5f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,
    1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  -0.5f,
    0.5f,  -0.5f, 0.0f,  1.0f};

vec3 cubePositions[] = {{0.0f, 0.0f, 0.0f},    {2.0f, 5.0f, -15.0f},
                        {-1.5f, -2.2f, -2.5f}, {-3.8f, -2.0f, -12.3f},
                        {2.4f, -0.4f, -3.5f},  {-1.7f, 3.0f, -7.5f},
                        {1.3f, -2.0f, -2.5f},  {1.5f, 2.0f, -2.5f},
                        {1.5f, 0.2f, -1.5f},   {-1.3f, 1.0f, -1.5f}};

/* unsigned int indices[] = { */
/*     // note that we start from 0! */
/*     0, 1, 3, // first triangle */
/*     1, 2, 3  // second triangle */
/* }; */

int main() {
  // This is a simple C program that prints "Hello, World!" to the console.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);

  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glEnable(GL_DEPTH_TEST);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int shaderProgram =
      generateShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  unsigned int shaderProgram2 =
      generateShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); */
  /* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, */
  /*              GL_STATIC_DRAW); */

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); */

  glBindVertexArray(0);

  unsigned int texture1 =
      generateTexture2D("resources/textures/container.jpg", GL_REPEAT,
                        GL_REPEAT, GL_LINEAR, GL_LINEAR);

  unsigned int texture2 =
      generateTexture2D("resources/textures/awesomeface.png", GL_REPEAT,
                        GL_REPEAT, GL_LINEAR, GL_LINEAR);

  glUseProgram(shaderProgram);
  glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
  glUniform1f(glGetUniformLocation(shaderProgram, "mixValue"), 0.2f);

  /* vec3 cameraPos = {0.0f, 0.0f, 3.0f}; */
  /* vec3 cameraTarget = {0.0f, 0.0f, 0.0f}; */
  /* vec3 cameraDirection; */
  /* glm_vec3_sub(cameraPos, cameraTarget, cameraDirection); */
  /* glm_normalize(cameraDirection); */
  /**/
  /* vec3 up = {0.0f, 1.0f, 0.0f}; */
  /* vec3 cameraRight; */
  /* glm_vec3_cross(up, cameraDirection, cameraRight); */
  /* vec3 cameraUp; */
  /* glm_vec3_cross(cameraDirection, cameraRight, cameraUp); */

  mat4 view;
  glm_lookat((vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 0.0f, 0.0f},
             (vec3){0.0f, 1.0f, 0.0f}, view);

  /* glm_rotate(model, glm_rad(-55.0f), (vec3){1.0f, 0.0f, 0.0f}); */
  mat4 projection;
  glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform1f(glGetUniformLocation(shaderProgram, "mixValue"), mixValue);

    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camY = cos(glfwGetTime()) * radius;
    mat4 view;
    glm_lookat((vec3){camX, 0.0f, camY}, (vec3){0.0f, 0.0f, 0.0f},
               (vec3){0.0f, 1.0f, 0.0f}, view);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                       GL_FALSE, (const GLfloat *)&projection);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                       (const GLfloat *)&view);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    /* glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */
    for (int i = 0; i < 10; i++) {
      mat4 model2 = GLM_MAT4_IDENTITY_INIT;
      glm_translate(model2, cubePositions[i]);

      float angle = 20.0f * i;
      if (i % 3 == 0)
        angle = glfwGetTime() * 25.0f;

      glm_rotate(model2, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
                         GL_FALSE, (const GLfloat *)&model2);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    /* glDrawArrays(GL_TRIANGLES, 0, 36); */

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    yAxis -= 0.01f;
    /* if (xAxis <= 0.0f) { */
    /*   xAxis = 0.0f; */
    /* } */
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    yAxis += 0.01f;
    /* if (xAxis >= 1.0f) { */
    /*   xAxis = 1.0f; */
    /* } */
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    xAxis -= 0.01f;
    /* if (xAxis <= 0.0f) { */
    /*   xAxis = 0.0f; */
    /* } */
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    xAxis += 0.01f;
    /* if (xAxis >= 1.0f) { */
    /*   xAxis = 1.0f; */
    /* } */
  }
}
