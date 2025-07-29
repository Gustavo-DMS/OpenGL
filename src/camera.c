#include <cglm/cglm.h>
#include "camera.h"

void camera_front(camera *cam) {
  vec3 direction;
  direction[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
  direction[1] = sin(glm_rad(cam->pitch));
  direction[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
  glm_vec3_normalize_to(direction, cam->front);
};

void camera_up(camera *cam) {
  glm_vec3_crossn(cam->right, cam->front, cam->up);
};

void camera_right(camera *cam) {
  glm_vec3_crossn(cam->front, cam->worldUp, cam->right);
};

void cameraRecalculateVectors(camera *cam) {
  camera_front(cam);
  camera_right(cam);
  camera_up(cam);
};

void cameraHandleMovement(camera *cam, enum CameraMovement movement,
                          float deltaTime, float speedModifier) {
  float speed = cam->movementSpeed * deltaTime * speedModifier;
  if (movement == FRONT) {
    glm_vec3_muladds(cam->front, speed, cam->position);
  }
  if (movement == BACK) {
    glm_vec3_mulsubs(cam->front, speed, cam->position);
  }
  if (movement == LEFT) {
    glm_vec3_mulsubs(cam->right, speed, cam->position);
  }
  if (movement == RIGHT) {
    glm_vec3_muladds(cam->right, speed, cam->position);
  }
  if (!cam->fly) {
    cam->position[1] = 0;
  }
};

void cameraHandleMouse(camera *cam, double xoffset, double yoffset,
                       bool RestrainPitch) {

  xoffset *= cam->sens;
  yoffset *= cam->sens;
  cam->yaw += xoffset;
  cam->pitch += yoffset;

  if (RestrainPitch) {
    if (cam->pitch > 89.0f)
      cam->pitch = 89.0f;
    if (cam->pitch < -89.0f)
      cam->pitch = -89.0f;
  }
  cameraRecalculateVectors(cam);
};

void cameraGetViewMatrix(camera *cam, mat4 dest) {
  vec3 sum;
  glm_vec3_add(cam->position, cam->front, sum);
  glm_lookat(cam->position, sum, cam->up, dest);
};
