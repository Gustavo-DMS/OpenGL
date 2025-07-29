#ifndef CAMERA_H
#define CAMERA_H
#include <cglm/cglm.h>

typedef struct Camera {
  // coords
  vec3 position;
  vec3 front;
  vec3 up;
  vec3 right;
  vec3 worldUp;

  // Euler angles
  float pitch;
  float yaw;
  float roll;

  // Params
  float fov;
  float movementSpeed;
  float sens;
  bool fly;
} camera;

enum CameraMovement { FRONT, BACK, LEFT, RIGHT };

void camera_front(camera *cam);

void camera_up(camera *cam);

void camera_right(camera *cam);

void cameraRecalculateVectors(camera *cam);

void cameraHandleMovement(camera *cam, enum CameraMovement movement,
                          float deltaTime, float speedModifier);

void cameraHandleMouse(camera *cam, double xoffset, double yoffset,
                       bool RestrainPitch);

void cameraGetViewMatrix(camera *cam, mat4 dest);

#endif
