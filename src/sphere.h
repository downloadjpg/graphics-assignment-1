#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include "ray.h"
#include "renderObject.h"

using namespace glm;

class Sphere : public RenderObject{
public:
    vec4 origin;
    float radius;

    IntersectionData intersection(Ray& ray);
    Sphere(vec4 _origin, float _radius);

};