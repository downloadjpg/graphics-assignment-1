#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "ray.h"
#pragma once

using namespace glm;


class Camera {
public:
    enum ProjectionType {orthographic, perspective};
    
    // Constructor
    Camera(vec3 _origin, vec3 _up, vec3 _forward);
    // Projection setting functions, can be orthographic or perspective. Perspective distance ~ FOV (kinda)
    void setProjectionType(ProjectionType type);
    void setPerspectiveDistance(float distance);

    void move(vec3 direction, float delta);
    void lookAt(vec3 position); // look at a point in (global) space e.g. `lookAt(sphere.origin)`
    // Generate a ray for a given pixel
    Ray generateRay(int i, int j);
    vec3 origin;

private:
    // Basis vectors for ray generation
    vec3 u;
    vec3 v;
    vec3 w;
    
    // canonical view volume dimensions (extract to struct?)
    // TODO: should the near plane be at 0???
    float l = -0.5f;
    float r = 0.5f;
    float b = -0.5f;
    float t = 0.5f;
    float f = -2.0f;
    float n = -1.0f;

    // render image dimensions in pixels
    // NOTE: a 1x1 view volume will stretch to fill a 2x1 image.
    int imageWidth = 512;
    int imageHeight = 512; // TODO: make this a parameter to the constructor

    ProjectionType projectionType = ProjectionType::perspective;
    float perspectiveDistance = 1.0f;
    Ray generateOrthographicRay(int i, int j);
    Ray generatePerspectiveRay(int i, int j);

};

