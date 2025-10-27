#include "../headers/Triangle.h"
#include "../headers/utils.h"


Triangle::Triangle(utils::Vec4 v0, utils::Vec4 v1, utils::Vec4 v2) {
    setV0(v0);
    setV1(v1);
    setV2(v2);

    setE01(v1 - v0);
    setE12(v2 - v1);
    setE20(v0 - v2);
}

void Triangle::setDiffuse(float r, float g, float b) {
    this->material.setDiffuse(r, g, b);
}
void Triangle::setSpecular(float r, float g, float b) {
    this->material.setSpecular(r, g, b);
}

