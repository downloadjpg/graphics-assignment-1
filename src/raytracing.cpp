#include "raytracing.h"
using namespace glm;


// Todo february 6th:
/*
    - Stop leaking memory every frame
    - Get the debugger working
    - Figure out why the plane won't render
    - Add a light source
    - Add a material system
    - Change your hit record to something smarter
*/


Color cuteGradient(float t) {
    // Cute little background gradient based off a given parameter [0.0,1.0]
    return Color(
        255 * (1-t),
        255 * (1-t),
        255
    );
}


Color rayColor(Ray& ray, std::vector<RenderObject*>& objects) {
    float closestIntersection = INFINITY;
    Color color = cuteGradient(ray.origin.y + 0.5); // background color!
    // Check for intersections with all objects
    for (RenderObject* object : objects) {

        // OPTIMIZE: we don't have to calculate the entire IntersectionData just to get the distance
        RenderObject::IntersectionData intersection = object->intersection(ray);
        if (!intersection.hit) {continue;}
        if (intersection.distance > closestIntersection) {continue;}

        closestIntersection = intersection.distance;

        
        // Color based on the normal of the intersection
        vec4& n = intersection.normal;
        n = (n * 0.5f + vec4(0.5, 0.5, 0.5, 0)) * 255.0f ;
        color = Color(n.x, n.y, n.z);
    }
    return color;
}

unsigned char* generateImage(const int width, const int height) {
    unsigned char* image = new unsigned char[width * height * 3];
    
    // Construct camera
    Camera camera = Camera(
        vec4(0,0,0,1), // origin
        vec4(0,1,0,0), // up
        vec4(0,0,-1,0) // forward
        );
    // Create a list of all renderable objects in the scene
    std::vector<RenderObject*> objects = {
        new Sphere(vec4(0.15,0,-8,1), 0.01f),
        new Sphere(vec4(-0.1,0,-10,1), 0.1f),
        new Sphere(vec4(0.1,0,-10,1), 0.1f),
        new Plane(vec4(0,-2.0,0,1), vec4(0,1,0,0))
        };

    // Loop through every pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++){
            // Generate a ray for the current pixel
            Ray ray = camera.generateRay(j, i); // j,i is x,y
            // Determine what color the ray ultimately contributes to the pixel
            Color c = rayColor(ray, objects);

            // Paint the pixel accordingly
            int idx = (i * width + j) * 3;
            image[idx] = c.r;
            image[idx+1] = c.g;
            image[idx+2] = c.b;

            }
        }
    // Finally, return the image
    return image;
}
