#include "ray.h"
#include "color.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#pragma once
using namespace glm;

class Surface {
public:
    struct HitRecord {
        bool hit;
        float distance;
        vec3 normal;
        vec3 position; //  redundant, as it's the distance times the ray's direction, but often needed eventually.
        Surface* surface; // oh boy now we're getting into memory management

        static HitRecord Miss() { return HitRecord{
            .hit = false,
            .distance = 0,
            .position = vec3(0,0,0),
            .surface = nullptr};
        }
    };

    struct Material {
        ColorF albedo = ColorF(0.7f,0.1f,0.2f); // bright annoying pink for uninitialized surfaces
        float specular = -1.0f; // 0-1000 are good values for this, negative means no specular reflection
    };


    vec3 origin;
    Material material;
    

    virtual HitRecord intersection(Ray& ray)  {return HitRecord::Miss();};
    virtual ~Surface() {};
};

// -----------------------------------------------------------------------------

class Sphere : public Surface {
public:
    vec3 origin;
    float radius;

    Sphere(vec3 _origin, float _radius);
    HitRecord intersection(Ray& ray);

};


class Plane : public Surface {
public:
    vec3 normal;

    Plane(vec3 position, vec3 _normal);
    HitRecord intersection(Ray& ray);
};



class Triangle : public Surface {
public:
    vec3 v1;
    vec3 v2;
    vec3 v3;
    vec3 normal;

    Triangle(vec3 _v1, vec3 _v2, vec3 _v3);
    HitRecord intersection(Ray& ray);
};


class Tetrahedron : public Surface {
public:
    vec3 origin;
    // vec3 edgeLength - might not be good to store this, as it's redundant with the vertices.
    Tetrahedron(vec3 _origin, float _edgeLength);
    ~Tetrahedron();
    HitRecord intersection(Ray& ray);
private:
    Triangle* t1;
    Triangle* t2;
    Triangle* t3;
    Triangle* t4;
};