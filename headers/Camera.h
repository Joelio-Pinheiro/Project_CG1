#ifndef CAMERA_H
#define CAMERA_H
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include <vector>


enum ProjectionType { PERSPECTIVE, ORTHOGRAPHIC, OBLIQUE };
class Camera {
    private:
        utils::Vec4 eye; // posiçao da camera
        utils::Vec4 forward; // vetor de direçao que a camera esta olhando
        utils::Vec4 up; // vetor acima da camera
        utils::Vec4 right; // vetor à direita da camera
        float pitch = 0.0f; // angulo de inclinaçao vertical
        ProjectionType projection = PERSPECTIVE;
    public:
        Camera(ProjectionType proj = PERSPECTIVE);

        void yaw(float angleDeg);
        void pitchRotate(float angleDeg);
        void move(float dx, float dy, float dz);

        void setEye(const utils::Vec4& e) { eye = e; reorthogonalize(); }
        void setForward(const utils::Vec4& f) { forward = f.normalize(); reorthogonalize(); }
        void setUp(const utils::Vec4& u) { up = u.normalize(); reorthogonalize(); }
        void setProjection(ProjectionType p) { projection = p; }

        utils::Vec4 getEye() const { return eye; }
        utils::Vec4 getForward() const { return forward; }
        utils::Vec4 getUp() const { return up; }
        ProjectionType getProjection() const { return projection; }

        utils::Vec4 getRight() const ;
    private:
        void reorthogonalize();
};


#endif // CAMERA_H