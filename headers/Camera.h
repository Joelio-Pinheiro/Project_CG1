#ifndef CAMERA_H
#define CAMERA_H
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include <vector>


class Camera {
    private:
        utils::Vec4 eye; // posiçao da camera
        utils::Vec4 forward; // vetor de direçao que a camera esta olhando
        utils::Vec4 up; // vetor acima da camera
        utils::Vec4 right; // vetor à direita da camera
    public:
        Camera();

        void rotate(float angleDeg, const utils::Vec4& axis);
        void move(float dx, float dy, float dz);

        void setEye(const utils::Vec4& eye);
        void setForward(const utils::Vec4& forward);
        void setUp(const utils::Vec4& up);

        utils::Vec4 getEye() const { return eye; }
        utils::Vec4 getForward() const { return forward; }
        utils::Vec4 getUp() const { return up; }
        utils::Vec4 getRight() const;
    private:
        void reorthogonalize();
};


#endif // CAMERA_H