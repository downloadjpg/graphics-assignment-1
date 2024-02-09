#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "ray.h"
#pragma once

using namespace glm;


class Camera {
public:
    enum ProjectionType {orthographic, perspective};
    
    // Constructor
    Camera(vec4 _origin, vec4 _up, vec4 _forward);
    // Projection setting functions, can be orthographic or perspective. Perspective distance ~ FOV (kinda)
    void setProjectionType(ProjectionType type);
    void setPerspectiveDistance(float distance);

    // Generate a ray for a given pixel
    Ray generateRay(int i, int j);
    vec4 origin;

private:
    // Basis vectors for ray generation
    vec4 u;
    vec4 v;
    vec4 w;
    
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

