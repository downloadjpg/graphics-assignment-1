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

    lights = {
        new Light(vec4(-4, 10, -10, 1), 1.0f)
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
    
    Surface::HitRecord closestHit;
    float closestDistance = INFINITY;

    Color color = Color(0,0,0);
    // Check for intersections with all objects
    for (Surface* surface : surfaces) {

        // OPTIMIZE: we don't have to calculate the entire IntersectionData just to get the distance
        if (surface == nullptr) {std::cout << "GAHH NULLPTR"; continue;}
        Surface::HitRecord intersection = surface->intersection(ray);
        if (!intersection.hit) {continue;}
        if (intersection.distance > closestDistance) {continue;}

        // If the intersection is closer than the previous closest, update the closest intersection
        closestDistance = intersection.distance;
        closestHit = intersection; // hopefully memory doens't leak here or something
        
    }
    // If the ray didn't intersect a single object, return the background color.
    if (closestDistance == INFINITY) {
        color = cuteGradient(ray.direction.y);
    }
    color = closestHit.surface->material.albedo * accumulateLight(ray, closestHit);
    return color;
}

 float Scene::accumulateLight(Ray& ray, Surface::HitRecord& hitRecord) {
    float totalLight = 0.0f;
    vec4 intersectionPosition = hitRecord.position;
    Surface* surface = hitRecord.surface;
    totalLight += 0.1f; // ambient light
    for (Light* light : lights) {
        // Diffuse!
        // calculate what direction the light is coming from
        vec4 incidentLightDirection = normalize(light->origin - intersectionPosition); //TODO: what should the w be here?
        float diffuse = max(0.0f, dot(hitRecord.normal, incidentLightDirection)); // multiply by constant here
        totalLight += diffuse;
        // Specular!
        // check if the material is specular at all
        if (surface->material.specular > 0) {
            vec4 reflection = reflect(ray.direction, hitRecord.normal);
            float specular = pow(max(0.0f, dot(reflection, incidentLightDirection)), surface->material.specular);
            totalLight += diffuse;
        }

    }
    return totalLight;
 }

Scene::~Scene() {
    delete camera;
    for (Surface* surface : surfaces) {
        delete surface;
    }
}