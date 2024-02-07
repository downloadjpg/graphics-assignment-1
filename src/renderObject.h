#include <glm/vec4.hpp>
#include "ray.h"
#include "color.h"
#pragma once

using namespace glm;


class RenderObject {
public:

    struct IntersectionData {
        bool hit;
        vec4 normal;
        float distance;
    };

    struct Material {
        Color albedo;
    };

    vec4 origin;
    virtual IntersectionData intersection(Ray& ray)  {
        return IntersectionData{
            .hit = false,
            .normal = vec4(0,0,0,0),
            .distance = 0
        };
    };
};
