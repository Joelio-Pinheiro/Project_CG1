#include "../headers/Camera.h"
#include "../headers/utils.h"
#include <algorithm>


Camera::Camera() {
    setEye(utils::Vec4::Point(0,0,0));
    setForward(utils::Vec4::Vector(0,0,-1));
    setUp(utils::Vec4::Vector(0,1,0));
    
    this->reorthogonalize();
}

void Camera::setEye(const utils::Vec4& eye) {
    this->eye = eye;
}
void Camera::setForward(const utils::Vec4& forward) {
    this->forward = forward.normalize();

    this->reorthogonalize();
}
void Camera::setUp(const utils::Vec4& up) {
    this->up = up.normalize();

    this->reorthogonalize();
}

utils::Vec4 Camera::getRight() const {
    utils::Vec4 f = this->forward.normalize();
    utils::Vec4 u = this->up.normalize();
    return u.prodVectorial(f * (-1)).normalize();   // up × forward
}


void Camera::yaw(float angleDeg) {
    float rad = angleDeg * M_PI / 180.0f;
    utils::Quaternion q = utils::Quaternion::fromAxisAngle(up, rad);

    forward = forward.rotateVec(q).normalize();
    reorthogonalize();
}


void Camera::pitchRotate(float angleDeg) {
    float newPitch = std::clamp(pitch + angleDeg, -89.0f, 89.0f);
    float delta = newPitch - pitch;
    pitch = newPitch;

    if (delta == 0.0f) return;

    float rad = delta * M_PI / 180.0f;
    utils::Quaternion q =
        utils::Quaternion::fromAxisAngle(right, rad);

    forward = forward.rotateVec(q).normalize();
    reorthogonalize();
}


void Camera::move(float dx, float dy, float dz) {
    eye = eye
        + right   * dx
        + up      * dy
        + forward * dz;
    printf("Camera position: "); eye.print();
}

void Camera::reorthogonalize() {
    forward = forward.normalize();
    right   = forward.prodVectorial(up).normalize();
    up      = right.prodVectorial(forward).normalize();
}
