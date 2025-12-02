#include "../headers/Camera.h"
#include "../headers/utils.h"


Camera::Camera() {
    setEye(utils::Vec4::Point(0,0,0));
    setForward(utils::Vec4::Vector(0,0,-1));
    setUp(utils::Vec4::Vector(0,1,0));
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
    return u.prodVectorial(f).normalize();   // up × forward
}


void Camera::rotate(float angleDeg, const utils::Vec4& axis) {
    float rad = angleDeg * M_PI / 180.0f;
    utils::Quaternion q = utils::Quaternion::fromAxisAngle(axis.normalize(), rad);

    this->forward = forward.rotateVec(q).normalize();

    this->up      = up.rotateVec(q).normalize();

    this->reorthogonalize();
}

void Camera::move(float dx, float dy, float dz) {
    utils::Vec4 t = utils::Vec4::Vector(dx, dy, dz);
    eye = eye + t;
}

void Camera::reorthogonalize() {
    utils::Vec4 f = this->forward.normalize();
    utils::Vec4 r = this->up.prodVectorial(f).normalize();
    utils::Vec4 u = f.prodVectorial(r).normalize();       

    this->forward = f;
    this->right   = r;
    this->up      = u;
}
