#include "surface.h"

Tetrahedron::Tetrahedron(vec3 _origin, float _edgeLength) {
    origin = _origin;
    edgeLength = _edgeLength;
    float r2 = 1.0f / sqrt(2.0f);

    v1 = origin + vec3(1.0f, 0.0f, -r2) * 0.5f * edgeLength;
    v2 = origin + vec3(-1.0f, 0.0f, -r2) * 0.5f * edgeLength;
    v3 = origin + vec3(0.0f, 1.0f, r2) * 0.5f * edgeLength;
    v4 = origin + vec3(0.0f, -1.0f, r2) * 0.5f * edgeLength;

    // The set of triangles is as follows
    t1 = new Triangle(v1, v2, v3);
    t2 = new Triangle(v1, v2, v4);
    t3 = new Triangle(v1, v3, v4);
    t4 = new Triangle(v2, v3, v4);
};

HitRecord Tetrahedron::intersection(Ray& ray) {
    HitRecord boundingCheck = Sphere(origin, edgeLength * 2.0f).intersection(ray);
    if (!boundingCheck.hit) {
        return HitRecord::Miss();
    }
    HitRecord hit1 = t1->intersection(ray);
    HitRecord hit2 = t2->intersection(ray);
    HitRecord hit3 = t3->intersection(ray);
    HitRecord hit4 = t4->intersection(ray);

    // Return the closest hit
    float closestDist = std::numeric_limits<float>::max();
    HitRecord closestHit = HitRecord::Miss();

    if (hit1.hit && hit1.distance < closestDist) {
        closestDist = hit1.distance;
        closestHit = hit1;
    }
    if (hit2.hit && hit2.distance < closestDist) {
        closestDist = hit2.distance;
        closestHit = hit2;
    }
    if (hit3.hit && hit3.distance < closestDist) {
        closestDist = hit3.distance;
        closestHit = hit3;
    }
    if (hit4.hit && hit4.distance < closestDist) {
        closestDist = hit4.distance;
        closestHit = hit4;
    }
    return closestHit;
}

Tetrahedron::~Tetrahedron() {
    delete t1;
    delete t2;
    delete t3;
    delete t4;
}
