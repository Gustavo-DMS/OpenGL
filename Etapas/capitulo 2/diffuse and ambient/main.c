#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "cglm/affine.h"
#include "stb_image.h"
#include "shader.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

float lastX = 400.0, lastY = 300.0;
int firstMouse = 1; // To check if the mouse is moved for the first time
struct Camera cam = {
    .position = {0.0f, 0.0f, 3.0f},
    .pitch = 0.0f,
    .movementSpeed = 2.5f,
    .worldUp = {0.0f, 1.0f, 0.0f},
    .front = {0.0f, 0.0f, -1.0f},
    .fly = 1,
    .sens = 0.1f,
    .fov = 0.45f,
    .yaw = -90.0f,
};

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float seconds = 0.0f;
int FrameCount = 0;

float mixValue = 0.2f;
float xAxis = 0.0f;
float yAxis = 0.0f;

vec3 lightPos = {1.2f, 1.0f, 2.0f};

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

/* unsigned int indices[] = { */
/*     // note that we start from 0! */
/*     0, 1, 3, // first triangle */
/*     1, 2, 3  // second triangle */
/* }; */

int main() {
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  cameraRecalculateVectors(&cam);

  unsigned int shaderCube =
      generateShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  unsigned int shaderLight = generateShaderProgram(
      "./shaders/vertex.glsl", "./shaders/fragmentLight.glsl");

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); */

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  /* glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), */
  /*                       (void *)(3 * sizeof(float))); */
  /* glEnableVertexAttribArray(1); */

  unsigned int texture1 =
      generateTexture2D("resources/textures/container.jpg", GL_REPEAT,
                        GL_REPEAT, GL_LINEAR, GL_LINEAR);

  unsigned int texture2 =
      generateTexture2D("resources/textures/awesomeface.png", GL_REPEAT,
                        GL_REPEAT, GL_LINEAR, GL_LINEAR);

  glUseProgram(shaderCube);
  /* glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); */
  /* glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1); */
  /* glUniform1f(glGetUniformLocation(shaderProgram, "mixValue"), 0.2f); */
  glUniform3f(glGetUniformLocation(shaderCube, "objectColor"), 1.0f, 0.5f,
              0.31f);
  glUniform3f(glGetUniformLocation(shaderCube, "lightColor"), 1.0f, 1.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (seconds >= 1.0f) {
      printf("FPS: %i\n", FrameCount);
      FrameCount = 0;
      seconds = 0.0f;
    }
    FrameCount++;
    seconds += deltaTime;

    mat4 view;
    cameraGetViewMatrix(&cam, view);

    mat4 projection;
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderCube);
    glUniformMatrix4fv(glGetUniformLocation(shaderCube, "projection"), 1,
                       GL_FALSE, (const GLfloat *)projection);
    glUniformMatrix4fv(glGetUniformLocation(shaderCube, "view"), 1, GL_FALSE,
                       (const GLfloat *)view);
    glUniform3f(glGetUniformLocation(shaderCube, "lightPos"), lightPos[0],
                lightPos[1], lightPos[2]);

    /* glActiveTexture(GL_TEXTURE0); */
    /* glBindTexture(GL_TEXTURE_2D, texture1); */
    /* glActiveTexture(GL_TEXTURE1); */
    /* glBindTexture(GL_TEXTURE_2D, texture2); */
    glBindVertexArray(VAO);
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glUniformMatrix4fv(glGetUniformLocation(shaderCube, "model"), 1, GL_FALSE,
                       (const GLfloat *)model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUseProgram(shaderLight);
    mat4 translate;
    glm_translate(model, lightPos);
    glm_scale(translate, (vec3){0.1f, 0.1f, 0.1f});
    glBindVertexArray(lightVAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderLight, "projection"), 1,
                       GL_FALSE, (const GLfloat *)projection);
    glUniformMatrix4fv(glGetUniformLocation(shaderLight, "view"), 1, GL_FALSE,
                       (const GLfloat *)view);
    glUniformMatrix4fv(glGetUniformLocation(shaderLight, "model"), 1, GL_FALSE,
                       (const GLfloat *)model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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
  float speedModifier = 1.0f;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speedModifier = 4.0f; // Increase speed when left shift is pressed
  }
  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
    printf("Enabled FlyMode");
    cam.fly = !cam.fly;
  }
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cameraHandleMovement(&cam, FRONT, deltaTime, speedModifier);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cameraHandleMovement(&cam, BACK, deltaTime, speedModifier);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cameraHandleMovement(&cam, LEFT, deltaTime, speedModifier);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cameraHandleMovement(&cam, RIGHT, deltaTime, speedModifier);
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = 0;
  }
  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;
  cameraHandleMouse(&cam, xoffset, yoffset, 1);
}
