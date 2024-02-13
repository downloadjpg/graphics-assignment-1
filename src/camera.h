#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "ray.h"
#include "surface.h"
#include "light.h"

using namespace glm;

class Camera {
public:
    enum ProjectionType {orthographic, perspective};

    // Constructor
    Camera(vec3 _origin, vec3 _up, vec3 _forward);    

    // Function for setting the projection type, FOV.
    void setProjectionType(ProjectionType type);
    void setPerspectiveFOV(float fov);

    void move(vec3 direction, float delta);
    void lookAt(vec3 position); // look at a point in (global) space e.g. `lookAt(sphere.origin)`
    void rotate(vec3 axis, float delta); // rotate around an axis (in global space) e.g. `rotate(vec3(0,1,0), 0.1f

    vec3 origin;
    int imageWidth = 512;
    int imageHeight = 512;
    
    // Generate a ray for a given pixel, depends on projection type, origin, and basis vectors.
    Ray generateRay(int i, int j);

private:
    // Basis vectors for ray generation
    vec3 u;
    vec3 v;
    vec3 w;
    
    // The in-world 'size' of the view plane
    const struct ViewPlane {
        float l = -0.5f * 5; // left
        float r = 0.5f * 5; // right
        float b = -0.5f * 5; // bottom
        float t = 0.5f * 5; // top
    } viewPlane;

    ProjectionType projectionType = ProjectionType::perspective;
    float perspectiveDistance = 5.0f; // TODO: incorporate FOV!

    Ray generateOrthographicRay(int i, int j);
    Ray generatePerspectiveRay(int i, int j);
};