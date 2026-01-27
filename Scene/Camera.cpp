#include "../headers/Camera.h"
#include "../headers/utils.h"
#include <algorithm>


Camera::Camera(ProjectionType proj) {
    setEye(utils::Vec4::Point(0,0,0));
    setForward(utils::Vec4::Vector(0,0,-1));
    setUp(utils::Vec4::Vector(0,1,0));
    this->projection = proj;
    
    this->reorthogonalize();
}


utils::Vec4 Camera::getRight() const {
    // right = forward × up
    utils::Vec4 f = forward.normalize();
    utils::Vec4 u = up.normalize();
    return f.prodVectorial(u).normalize();
}


void Camera::yaw(float angleDeg) {
    float rad = angleDeg * M_PI / 180.0f;
    utils::Quaternion q = utils::Quaternion::fromAxisAngle(up.normalize(), rad);

    forward = forward.rotateVec(q).normalize();
    up      = up.rotateVec(q).normalize();

    reorthogonalize();
}



void Camera::pitchRotate(float angleDeg) {
    float newPitch = std::clamp(pitch + angleDeg, -89.0f, 89.0f);
    float delta = newPitch - pitch;
    const float EPS = 1e-4f;
    if (std::fabs(delta) < EPS) return;
    pitch = newPitch;

    utils::Vec4 axis = this->getRight(); 
    float rad = delta * M_PI / 180.0f;
    utils::Quaternion q = utils::Quaternion::fromAxisAngle(axis.normalize(), rad);

    forward = forward.rotateVec(q).normalize();
    up      = up.rotateVec(q).normalize();

    reorthogonalize();
}



void Camera::move(float dx, float dy, float dz) {
    eye = eye
        + right   * dx
        + up      * dy
        + forward * dz;
}

void Camera::reorthogonalize() {
    forward = forward.normalize();
    right   = forward.prodVectorial(up).normalize();  // forward × up
    up      = right.prodVectorial(forward).normalize(); // right × forward
}
