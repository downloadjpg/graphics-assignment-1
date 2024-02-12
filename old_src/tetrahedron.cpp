#include "surface.h"

Tetrahedron::Tetrahedron(vec3 _origin, float _edgeLength) {
    float r2 = 1.0f / sqrt(2.0f);
    vec3 v1 = vec3(1.0, 0.0, -r2);
    vec3 v2 = vec3(-1.0, 0.0, -r2);
    vec3 v3 = vec3(0.0, 1.0, r2);
    vec3 v4 = vec3(0.0, -1.0, r2);

    //Therefore, the set of triangles is as follows
    t1 = new Triangle(v1, v2, v3);
    t2 = new Triangle(v1, v2, v4);
    t3 = new Triangle(v1, v3, v4);
    t4 = new Triangle(v2, v3, v4);
};

Tetrahedron::HitRecord Tetrahedron::intersection(Ray& ray) {
    HitRecord hit1 = t1->intersection(ray);
    HitRecord hit2 = t2->intersection(ray);
    HitRecord hit3 = t3->intersection(ray);
    HitRecord hit4 = t4->intersection(ray);

    if (hit1.hit) {
        return hit1;
    }
    if (hit2.hit) {
        return hit2;
    }
    if (hit3.hit) {
        return hit3;
    }
    if (hit4.hit) {
        return hit4;
    }
    return HitRecord::Miss();
}

Tetrahedron::~Tetrahedron() {
    delete t1;
    delete t2;
    delete t3;
    delete t4;
}

// Create tetrahedron
// define an origin, there's some symmetry here but i'm not sure.

// central triangle facing up,
// vertex coordinates are in https://en.wikipedia.org/wiki/Tetrahedron


