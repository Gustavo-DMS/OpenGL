#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "stb_image.h"
#include "shader.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

float lastX = 400.0, lastY = 300.0;
float windowWidth = 800, windowHeight = 600;
int firstMouse = 1; // To check if the mouse is moved for the first time
bool flashLight = false;
vec3 lightIntensity = {1.0f, 1.0f, 1.0f};

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

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};
struct PointLight pointLights[4] = {
    {
        .ambient = {0.05f, 0.05f, 0.05f},
        .diffuse = {0.8f, 0.8f, 0.8f},
        .specular = {1.0f, 1.0f, 1.0f},
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032f,
        .position = {0.7f, 0.2f, 2.0f},
    },
    {
        .ambient = {0.05f, 0.05f, 0.05f},
        .diffuse = {0.8f, 0.8f, 0.8f},
        .specular = {1.0f, 1.0f, 1.0f},
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032f,
        .position = {2.3f, -3.3f, -4.0f},
    },
    {
        .ambient = {0.05f, 0.05f, 0.05f},
        .diffuse = {0.8f, 0.8f, 0.8f},
        .specular = {1.0f, 1.0f, 1.0f},
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032f,
        .position = {-4.0f, 2.0f, -12.0f},
    },
    {
        .ambient = {0.05f, 0.05f, 0.05f},
        .diffuse = {0.8f, 0.8f, 0.8f},
        .specular = {1.0f, 1.0f, 1.0f},
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032f,
        .position = {0.0f, 0.0f, -3.0f},
    },
};

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float seconds = 0.0f;
int FrameCount = 0;

float mixValue = 0.2f;
float xAxis = 0.0f;
float yAxis = 0.0f;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

// positions all containers
vec3 cubePositions[] = {{0.0f, 0.0f, 0.0f},    {2.0f, 5.0f, -15.0f},
                        {-1.5f, -2.2f, -2.5f}, {-3.8f, -2.0f, -12.3f},
                        {2.4f, -0.4f, -3.5f},  {-1.7f, 3.0f, -7.5f},
                        {1.3f, -2.0f, -2.5f},  {1.5f, 2.0f, -2.5f},
                        {1.5f, 0.2f, -1.5f},   {-1.3f, 1.0f, -1.5f}};

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwSwapInterval(0);
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
  glfwSetKeyCallback(window, key_callback);

  cameraRecalculateVectors(&cam);

  unsigned int shaderCube =
      generateShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  unsigned int shaderLight = generateShaderProgram(
      "./shaders/vertex.glsl", "./shaders/fragmentLight.glsl");

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); */

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  /* glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), */
  /*                       (void *)(3 * sizeof(float))); */
  /* glEnableVertexAttribArray(1); */

  unsigned int textureDiffuse =
      generateTexture2D("resources/textures/container2.png", GL_REPEAT,
                        GL_REPEAT, GL_LINEAR, GL_LINEAR);

  unsigned int textureSpecular =
      generateTexture2D("resources/textures/container2_specular.png", GL_REPEAT,
                        GL_REPEAT, GL_LINEAR, GL_LINEAR);
  unsigned int textureEmission =
      generateTexture2D("resources/textures/matrix.jpg", GL_REPEAT, GL_REPEAT,
                        GL_LINEAR, GL_LINEAR);

  glUseProgram(shaderCube);
  glUniform1i(glGetUniformLocation(shaderCube, "material.diffuse"), 0);
  glUniform1i(glGetUniformLocation(shaderCube, "material.specular"), 1);
  glUniform1i(glGetUniformLocation(shaderCube, "material.emission"), 2);

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
    glm_perspective(glm_rad(45.0f), windowWidth / windowHeight, 0.1f, 100.0f,
                    projection);

    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 model = GLM_MAT4_IDENTITY_INIT;

    glUseProgram(shaderLight);
    mat4 translate = GLM_MAT4_IDENTITY_INIT;
    /* vec3 lightPos = {sin(currentFrame), 1.0f, cos(currentFrame)}; */

    for (int i = 0; i < 4; i++) {
      vec3 lightPos;
      glm_vec3_copy(pointLights[i].position, lightPos);
      glm_translate(translate, lightPos);
      glm_scale(translate, (vec3){0.2f, 0.2f, 0.2f});
      glUniformMatrix4fv(glGetUniformLocation(shaderLight, "projection"), 1,
                         GL_FALSE, (const GLfloat *)projection);
      glUniformMatrix4fv(glGetUniformLocation(shaderLight, "view"), 1, GL_FALSE,
                         (const GLfloat *)view);
      glUniformMatrix4fv(glGetUniformLocation(shaderLight, "model"), 1,
                         GL_FALSE, (const GLfloat *)translate);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glm_mat4_identity(translate); // Reset translate for next light
    }

    glBindVertexArray(VAO);
    glUseProgram(shaderCube);
    // view matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderCube, "projection"), 1,
                       GL_FALSE, (const GLfloat *)projection);
    glUniformMatrix4fv(glGetUniformLocation(shaderCube, "view"), 1, GL_FALSE,
                       (const GLfloat *)view);
    glUniform3f(glGetUniformLocation(shaderCube, "viewPos"), cam.position[0],
                cam.position[1], cam.position[2]);
    glUniformMatrix4fv(glGetUniformLocation(shaderCube, "model"), 1, GL_FALSE,
                       (const GLfloat *)model);

    // material properties
    glUniform1f(glGetUniformLocation(shaderCube, "material.shininess"), 32.0f);

    // dir light
    glUniform3fv(glGetUniformLocation(shaderCube, "directionalLight.direction"),
                 1, (vec3){-0.2f, -1.0f, -0.3f});
    glUniform3fv(glGetUniformLocation(shaderCube, "directionalLight.ambient"),
                 1, (vec3){0.05f, 0.05f, 0.05f});
    glUniform3fv(glGetUniformLocation(shaderCube, "directionalLight.diffuse"),
                 1, (vec3){0.4f, 0.4f, 0.4f});
    glUniform3fv(glGetUniformLocation(shaderCube, "directionalLight.diffuse"),
                 1, (vec3){0.5f, 0.5f, 0.5f});

    glUniform1i(glGetUniformLocation(shaderCube, "pointLightCount"), 4);
    for (int i = 0; i < 4; i++) {
      char uniformName[64];
      snprintf(uniformName, sizeof(uniformName), "pointLights[%d].position", i);
      glUniform3fv(glGetUniformLocation(shaderCube, uniformName), 1,
                   pointLights[i].position);
      snprintf(uniformName, sizeof(uniformName), "pointLights[%d].ambient", i);
      glUniform3fv(glGetUniformLocation(shaderCube, uniformName), 1,
                   pointLights[i].ambient);
      snprintf(uniformName, sizeof(uniformName), "pointLights[%d].diffuse", i);
      glUniform3fv(glGetUniformLocation(shaderCube, uniformName), 1,
                   pointLights[i].diffuse);
      snprintf(uniformName, sizeof(uniformName), "pointLights[%d].specular", i);
      glUniform3fv(glGetUniformLocation(shaderCube, uniformName), 1,
                   pointLights[i].specular);
      snprintf(uniformName, sizeof(uniformName), "pointLights[%d].constant", i);
      glUniform1f(glGetUniformLocation(shaderCube, uniformName),
                  pointLights[i].constant);
      snprintf(uniformName, sizeof(uniformName), "pointLights[%d].linear", i);
      glUniform1f(glGetUniformLocation(shaderCube, uniformName),
                  pointLights[i].linear);
      snprintf(uniformName, sizeof(uniformName), "pointLights[%d].quadratic",

               i);
      glUniform1f(glGetUniformLocation(shaderCube, uniformName),
                  pointLights[i].quadratic);
    }

    // spot light
    glUniform3fv(glGetUniformLocation(shaderCube, "spotLight.position"), 1,
                 cam.position);
    glUniform3fv(glGetUniformLocation(shaderCube, "spotLight.direction"), 1,
                 cam.front);
    glUniform3fv(glGetUniformLocation(shaderCube, "spotLight.ambient"), 1,
                 (vec3){0.0f, 0.0f, 0.0f});
    glUniform3fv(glGetUniformLocation(shaderCube, "spotLight.diffuse"), 1,
                 lightIntensity);
    glUniform3fv(glGetUniformLocation(shaderCube, "spotLight.specular"), 1,
                 lightIntensity);
    glUniform1f(glGetUniformLocation(shaderCube, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderCube, "spotLight.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(shaderCube, "spotLight.quadratic"),
                0.032f);
    glUniform1f(glGetUniformLocation(shaderCube, "spotLight.cutOff"),
                cos(glm_rad(12.5f)));
    glUniform1f(glGetUniformLocation(shaderCube, "spotLight.outerCutOff"),
                cos(glm_rad(15.0f)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureDiffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureSpecular);
    /* glActiveTexture(GL_TEXTURE2); */
    /* glBindTexture(GL_TEXTURE_2D, textureEmission); */

    for (int i = 0; i < 10; i++) {
      mat4 model2 = GLM_MAT4_IDENTITY_INIT;
      glm_translate(model2, cubePositions[i]);
      float angle = 20.0f * i;
      glm_rotate(model2, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
      glUniformMatrix4fv(glGetUniformLocation(shaderCube, "model"), 1, GL_FALSE,
                         (const GLfloat *)&model2);
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
  windowWidth = width;
  windowHeight = height;
}

void processInput(GLFWwindow *window) {
  float speedModifier = 1.0f;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speedModifier = 4.0f; // Increase speed when left shift is pressed
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

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_V && action == GLFW_PRESS) {
    printf("Enabled FlyMode\n");
    cam.fly = !cam.fly;
  }
  if (key == GLFW_KEY_F && action == GLFW_PRESS) {
    printf("Toggle Flashlight\n");
    if (flashLight) {
      lightIntensity[0] = 0.0f;
      lightIntensity[1] = 0.0f;
      lightIntensity[2] = 0.0f;
    } else {
      lightIntensity[0] = 1.0f;
      lightIntensity[1] = 1.0f;
      lightIntensity[2] = 1.0f;
    }
    flashLight = !flashLight;
  }
}
