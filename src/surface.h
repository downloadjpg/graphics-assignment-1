#pragma once
#include "ray.h"
#include "hitRecord.h"
#include <glm/vec3.hpp>
class Surface {
public:
// Material properties, can be used by other classes, but it's a surface property.
    struct Material {
        vec3 albedo = vec3(0.7f,0.1f,0.2f); // bright annoying pink for uninitialized surfaces
        float specular = -1.0f; // 0-1000 are good values for this, negative means no specular reflection
        float reflective = 0.0f; // TODO: float?
    };

    vec3 origin;
    Material material = Material();

    virtual HitRecord intersection(Ray& ray)  {return HitRecord::Miss();};
    virtual ~Surface() {};    
};


// Define a few surfaces!

class Sphere : public Surface {
public:
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
    // vec3 edgeLength - might not be good to store this, as it's redundant with the vertices.
    Tetrahedron(vec3 _origin, float _edgeLength);
    HitRecord intersection(Ray& ray);
private:
    vec3 v1, v2, v3, v4;
    float edgeLength;
    Triangle t1 = Triangle(v1, v2, v3);
    Triangle t2 = Triangle(v1, v2, v4);
    Triangle t3 = Triangle(v1, v3, v4);
    Triangle t4 = Triangle(v2, v3, v4);
};