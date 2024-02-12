#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "surface.h"

class Triangle : public Surface {
public:
vec3 v1;
vec3 v2;
vec3 v3;
    // Constructor
    Triangle(vec3 _v1, vec3 _v2, vec3 _v3);
    HitRecord intersection(Ray& ray);
private:
    // Barycentric something??
};

#endif // TRIANGLE_H
