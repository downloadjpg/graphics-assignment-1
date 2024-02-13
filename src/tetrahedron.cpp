#include "surface.h"
#include <vector>

Tetrahedron::Tetrahedron(vec3 _origin, float _edgeLength) {
    origin = _origin;
    edgeLength = _edgeLength;
    float r2 = 1.0f / sqrt(2.0f);

    v1 = origin + vec3(1.0f, 0.0f, -r2) * 0.5f * edgeLength;
    v2 = origin + vec3(-1.0f, 0.0f, -r2) * 0.5f * edgeLength;
    v3 = origin + vec3(0.0f, 1.0f, r2) * 0.5f * edgeLength;
    v4 = origin + vec3(0.0f, -1.0f, r2) * 0.5f * edgeLength;

    // The set of triangles is as follows
    triangles = {
        new Triangle(v1, v2, v3),
        new Triangle(v1, v2, v4),
        new Triangle(v1, v3, v4),
        new Triangle(v2, v3, v4)
    };

    // Make sure the normals are right!
    for (auto triangle : triangles) {
        if (dot(triangle->normal, v1 - origin) < 0) {
            triangle->normal = -triangle->normal;
        }
    }
    


};

HitRecord Tetrahedron::intersection(Ray& ray) {
    HitRecord boundingCheck = Sphere(origin, edgeLength * 2.0f).intersection(ray);
    if (!boundingCheck.hit) {
        return HitRecord::Miss();
    }

    HitRecord closestHit = HitRecord::Miss();
    closestHit.distance = std::numeric_limits<float>::infinity();
    for (auto triangle : triangles) {
        HitRecord hit = triangle->intersection(ray);
        if (hit.hit && hit.distance < closestHit.distance) {
            closestHit = hit;
        }
    }

    return HitRecord{
        .hit = closestHit.hit,
        .distance = closestHit.distance,
        .normal = closestHit.normal,
        .position = closestHit.position,
        .surface = this
    };
};

Tetrahedron::~Tetrahedron() {
    for (auto triangle : triangles) {
        delete triangle;
    }
}
