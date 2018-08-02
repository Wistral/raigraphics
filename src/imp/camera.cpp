//
// Created by jhwangbo on 17. 4. 27.
//
#include <raiGraphics/RAI_keyboard.hpp>
#include "raiGraphics/imp/camera.h"

namespace rai_graphics {

void Camera::lockCamera() {
  if (camPitch > 90)
    camPitch = 90;
  if (camPitch < -90)
    camPitch = -90;
  if (camYaw < 0.0)
    camYaw += 360.0;
  if (camYaw > 360.0)
    camYaw -= 360;
}

Camera::Camera(const glm::vec3 &posI, float fov, float aspect, float zNear, float zFar) {
  pos = posI;
  forward = glm::vec3(0.0f, 0.0f, 1.0f);
  up = glm::vec3(0.0f, 0.0f, 1.0f);
  projection = glm::perspective(fov, aspect, zNear, zFar);
  camPitch = 0;
  camYaw = 0;
  camAngularSpeed = 0.2;
  camLinearSpeed = 0.1;
  keyState = SDL_GetKeyboardState(NULL);
}

void Camera::update() {
  mtx.lock();

  float sinYaw = sin(camYaw / 180.f * M_PI);
  float cosYaw = cos(camYaw / 180.f * M_PI);
  float sinPitch = sin(camPitch / 180.f * M_PI);
  float cosPitch = cos(camPitch / 180.f * M_PI);

  float lx = cosYaw * cosPitch;
  float ly = sinYaw * cosPitch;
  float lz = sinPitch;

  glm::vec3 offset;
  offset.x = lx * 10;
  offset.y = ly * 10;
  offset.z = lz * 10;

  if(!toFollowObj)
    mi = true;

  if (mi) {
    glm::vec3 alternateUp(1, 0, 0);
    pose_ = glm::lookAt(pos, pos + offset, up);
  } else {
    Transform trans;
    toFollowObj->getTransform(trans);
    glm::vec3 camPos = *trans.GetPos() + glm::vec3(relativePos);
    pose_ = glm::lookAt(camPos, *trans.GetPos(), up);
  }
  vp_ = projection * pose_;
  mtx.unlock();
}

void Camera::GetVP(glm::mat4 &vp) {
  mtx.lock();
  vp = vp_;
  mtx.unlock();
}

void Camera::GetPose(glm::mat4 &pose) {
  mtx.lock();
  pose = pose_;
  mtx.unlock();
}

void Camera::GetPos(glm::vec3 &position) {
  mtx.lock();
  if (mi)
    position = pos;
  else{
    Transform trans;
    toFollowObj->getTransform(trans);
    position = *trans.GetPos() + glm::vec3(relativePos);
  }
  mtx.unlock();
}

void Camera::Control(SDL_Event e, bool stayAboveZero) {
  std::lock_guard<std::mutex> guad(mtx);
  if(!toFollowObj)
    mi = true;

  if (mi && !keyState[RAI_KEY_LCTRL]) {
    float sinYaw = sin(camYaw / 180 * M_PI);
    float cosYaw = cos(camYaw / 180 * M_PI);
    float sinPitch = sin(camPitch / 180 * M_PI);
    float cosPitch = cos(camPitch / 180 * M_PI);

    float lx = cosYaw * cosPitch;
    float ly = sinYaw * cosPitch;
    float lz = sinPitch;

    glm::vec3 panUp = glm::normalize(glm::vec3(0,0,1) - glm::dot(glm::vec3(lx, ly, lz), glm::vec3(0,0,1)) / glm::dot(glm::vec3(lx, ly, lz), glm::vec3(lx, ly, lz)) * glm::vec3(lx, ly, lz));
    glm::vec3 panRight = glm::normalize(glm::cross(panUp, glm::vec3(lx, ly, lz)));

    int tmpx = 0, tmpy = 0;
    SDL_GetMouseState(&tmpx, &tmpy);

    if (mousePressedLastTimeStep) {
      camYaw += camAngularSpeed * (tmpx - prevMx);
      camPitch += camAngularSpeed * (tmpy - prevMy);
      camPitch = std::min(std::max(camPitch, -89.5f), 89.5f);
    }

    if (mouseRightPressedLastTimeStep) {
      pos += camLinearSpeed * (tmpx - prevMx) * 0.2f * panRight;
      pos += camLinearSpeed * (tmpy - prevMy) * 0.2f * panUp;
    }

    lockCamera();
    glm::vec3 left, right;

    if (keyState[SDL_SCANCODE_W])
      pos += camLinearSpeed * glm::vec3(lx, ly, lz);

    if (keyState[SDL_SCANCODE_S])
      pos -= camLinearSpeed * glm::vec3(lx, ly, lz);

    if (keyState[SDL_SCANCODE_A]) {
      left = -camLinearSpeed * glm::normalize(glm::cross(glm::vec3(lx, ly, lz), up));
      pos += left;
    }

    if (keyState[SDL_SCANCODE_D]) {
      right = camLinearSpeed * glm::normalize(glm::cross(glm::vec3(lx, ly, lz), up));
      pos += right;
    }

    if (keyState[SDL_SCANCODE_KP_PLUS] || keyState[SDL_SCANCODE_P])
      camLinearSpeed *= 1.05;

    if (keyState[SDL_SCANCODE_KP_MINUS] || keyState[SDL_SCANCODE_O])
      camLinearSpeed /= 1.05;
  } else if(!keyState[RAI_KEY_LCTRL]) {

    int tmpx = 0, tmpy = 0;

    glm::vec4 rotationPitcAxis( -relativePos.y,relativePos.x, 0, 0), rotationPitcAxisnew;
    glm::normalize(rotationPitcAxis);

    if (mousePressedLastTimeStep) {
      SDL_GetMouseState(&tmpx, &tmpy);
      relativePos = glm::rotate(glm::radians((prevMx - tmpx) * 0.13f), glm::vec3(0, 0, 1)) * relativePos;
      relativePos = glm::rotate(glm::radians((prevMy - tmpy) * 0.13f), glm::vec3(rotationPitcAxis)) * relativePos;
//      if(stayAboveZero)
//        relativePos[2] = std::max(0.05f, relativePos[2]);
    }

    if (switchTime > 3) {
      if (keyState[SDL_SCANCODE_LEFT]) {
        relativePos = glm::rotate(glm::radians(22.5f), glm::vec3(0, 0, 1)) * relativePos;
        switchTime = 0;
      } else if (keyState[SDL_SCANCODE_RIGHT]) {
        relativePos = glm::rotate(glm::radians(-22.5f), glm::vec3(0, 0, 1)) * relativePos;
        switchTime = 0;
      } else if (keyState[SDL_SCANCODE_UP]) {
        relativePos = glm::rotate(glm::radians(-22.5f), glm::vec3(rotationPitcAxis)) * relativePos;
        switchTime = 0;
      } else if (keyState[SDL_SCANCODE_DOWN]) {
        relativePos = glm::rotate(glm::radians(-22.5f), glm::vec3(rotationPitcAxis)) * relativePos;
        switchTime = 0;
      }
    }
  }

  Uint32 mbuttonState = SDL_GetMouseState(&prevMx, &prevMy);
  if (mbuttonState == SDL_BUTTON_LEFT)
    mousePressedLastTimeStep = true;
  else
    mousePressedLastTimeStep = false;

  if (mbuttonState == SDL_BUTTON_RIGHT || mbuttonState == SDL_BUTTON_X1)
    mouseRightPressedLastTimeStep = true;
  else
    mouseRightPressedLastTimeStep = false;

  if (keyState[SDL_SCANCODE_SPACE] && switchTime > 10) {
    if (!toFollowObj) {
      std::cout << "specify which object to follow" << std::endl;
      return;
    }
    if(!mi)
      stayHere();
    mi = !mi;
    switchTime = 0;
  }

  switchTime++;
}

void Camera::setPose(const Eigen::Vector3d& pos_in, float yaw_in, float pitch_in) {
  mi = true;
  pos[0] = pos_in[0];
  pos[1] = pos_in[1];
  pos[2] = pos_in[2];
  camYaw = yaw_in;
  camPitch = pitch_in;
}

void Camera::follow(object::SingleBodyObject *obj, Eigen::Vector3d pos) {
  mtx.lock();
  toFollowObj = obj;
  relativePos = glm::vec4(pos[0], pos[1], pos[2], 0);
  mi = false;
  mtx.unlock();
}

void Camera::follow(object::SingleBodyObject *obj) {
  mtx.lock();
  toFollowObj = obj;
  mi = false;
  mtx.unlock();
}

void Camera::zoomIn() {
  mtx.lock();
  relativePos.x /= 1.1;
  relativePos.y /= 1.1;
  relativePos.z /= 1.1;
  mtx.unlock();
}

void Camera::zoomOut() {
  mtx.lock();
  relativePos.x *= 1.1;
  relativePos.y *= 1.1;
  relativePos.z *= 1.1;
  mtx.unlock();
}

object::SingleBodyObject* Camera::getToFollowObj() {
  return toFollowObj;
}

void Camera::unFollowOb() {
  mi=true;
  stayHere();
  toFollowObj = nullptr;
};

void Camera::stayHere() {
  Transform trans;
  toFollowObj->getTransform(trans);
  pos = *trans.GetPos() + glm::vec3(relativePos);
  float xdiff = -relativePos.x;
  float ydiff = -relativePos.y;
  float zdiff = -relativePos.z;
  float horizon = sqrt(xdiff*xdiff + ydiff*ydiff);

  camPitch = std::atan2(zdiff, horizon) / M_PI * 180;
  camYaw = std::atan2(ydiff, xdiff) / M_PI * 180;
}


} // rai_graphics