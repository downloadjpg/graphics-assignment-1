#include <glm/vec4.hpp>
#include "ray.h"
#include "color.h"
#pragma once
using namespace glm;

class Surface {
public:
    struct IntersectionData {
        bool hit;
        vec4 normal;
        float distance;
    };

    struct Material {
        Color albedo = Color(255,150,255); // bright annoying pink for uninitialized surfaces
        float specular; // example, not implemented
        float roughness; // example, not implemented
    };

    vec4 origin;
    Material material;
    
    virtual IntersectionData intersection(Ray& ray)  {
        return IntersectionData{
            .hit = false,
            .normal = vec4(0,0,0,0),
            .distance = 0
        };
    };
    virtual ~Surface() {};
};
