#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "ray.h"
#pragma once

using namespace glm;


class Camera {
    vec4 origin;
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

public:
    Ray generateRay(int i, int j);


public:
    Camera(vec4 _origin = vec4(0,0,0,1), vec4 _up = vec4(0,1,0,0), vec4 _forward = vec4(0,0,-1,0));
};

