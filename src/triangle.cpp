#include "triangle.h"
Triangle::Triangle(vec3 _v1, vec3 _v2, vec3 _v3) {
    v1 = _v1;
    v2 = _v2;
    v3 = _v3;
}

Triangle::HitRecord Triangle::intersection(Ray& ray) {
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    return HitRecord::Miss();
}


// Create tetrahedron
// define an origin, there's some symmetry here but i'm not sure.

// central triangle facing up,
// vertex coordinates are in https://en.wikipedia.org/wiki/Tetrahedron