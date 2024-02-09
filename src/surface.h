#include <glm/vec4.hpp>
#include "ray.h"
#include "color.h"
#pragma once
using namespace glm;

class Surface {
public:
    struct HitRecord {
        bool hit;
        float distance;
        vec4 normal;
        vec4 position; //  redundant, as it's the distance times the ray's direction, but often needed eventually.
        Surface* surface; // oh boy now we're getting into memory management

        static HitRecord Miss() { return HitRecord{
            .hit = false,
            .distance = 0,
            .normal = vec4(0,0,0,0),
            .position = vec4(0,0,0,0),
            .surface = nullptr};
        }
    };

    struct Material {
        Color albedo = Color(255,10,200); // bright annoying pink for uninitialized surfaces
        float specular = -1.0f; // 0-1000 are good values for this, negative means no specular reflection
    };

    vec4 origin;
    Material material;
    
    virtual HitRecord intersection(Ray& ray)  {
        return HitRecord{
            .hit = false,
            .distance = 0,
            .normal = vec4(0,0,0,0),
            .position = vec4(0,0,0,0),
            .surface = this
        };
    };
    virtual ~Surface() {};
};
