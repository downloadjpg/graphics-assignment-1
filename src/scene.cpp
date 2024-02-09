#include "scene.h"
#include <iostream>


Color cuteGradient(float t) {
    // Cute little background gradient based off a given parameter [0.0,1.0]
    return Color(
        255 * (1-t),
        255 * (1-t),
        255
    );
}


Scene::Scene() {

    time = 0.0f;

    camera = new Camera(
        vec4(0,0,0,1), // origin
        vec4(0,1,0,0), // up
        vec4(0,0,-1,0) // forward
    );

    surfaces = {
        new Sphere(vec4(-20,0,-12,1), 0.01f),
        new Sphere(vec4(-0.1,0,-5,1), 1.0f),
        new Sphere(vec4(0.1,0,-10,1), 1.0f),
        new Plane(vec4(0,-2.0,-20,1), normalize(vec4(0,1,0,0)))
    };
}


unsigned char* Scene::renderImage(const int width, const int height) {
    // Image array to be returned to the actual rasterizer.
    unsigned char* image = new unsigned char[width * height * 3];
    // Loop through every pixel in the image
    for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

        // Generate a ray for the current pixel
        Ray ray = camera->generateRay(j, i); // j,i is x,y
        // Determine what color the ray ultimately contributes to the pixel
        Color c = rayColor(ray);

        // Paint the pixel accordingly
        int idx = (i * width + j) * 3;
        image[idx] = c.r;
        image[idx+1] = c.g;
        image[idx+2] = c.b;

    }}
    // Finally, return the full image
    return image;
}


Color Scene::rayColor(Ray& ray) {
    float closestIntersection = INFINITY;
    Color color = cuteGradient(ray.direction.y + 0.5); // background color!
    // Check for intersections with all objects
    for (Surface* surface : surfaces) {

        // OPTIMIZE: we don't have to calculate the entire IntersectionData just to get the distance
        if (surface == nullptr) {std::cout << "GAHH NULLPTR"; continue;}
        Surface::IntersectionData intersection = surface->intersection(ray);
        if (!intersection.hit) {continue;}
        if (intersection.distance > closestIntersection) {continue;}

        closestIntersection = intersection.distance;

        
        // Color based on the normal of the intersection
        //vec4& n = intersection.normal;
        //n = (n * 0.5f + vec4(0.5, 0.5, 0.5, 0)) * 255.0f ;
        //color = Color(n.x, n.y, n.z);

        // Color based on the surface's albedo
        //color = surface->material.albedo;

        // Color based on depth (intersection distance)
        float max = 10.0f;
        float d = intersection.distance > max ? max : intersection.distance;
        d = d / max;
        d = d*d;
        color = Color(255 * (1 - (d)), 255 * (1 - (d)), 255 * (1 - (d)));
    }
    return color;
}

Scene::~Scene() {
    delete camera;
    for (Surface* surface : surfaces) {
        delete surface;
    }
}