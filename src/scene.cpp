#include "scene.h"
#include <iostream>

Scene::Scene() {

    camera = new Camera(
        vec3(0,0,0), // origin
        vec3(0,1,0), // up
        vec3(0,0,-1) // forward
    );

    surfaces = {
        new Sphere(vec3(1,0,-5), 0.2f),
        new Sphere(vec3(0,0,-8), 0.2f),
        new Sphere(vec3(-1,0,-5), 0.4f),
        new Plane(vec3(0,-2.0,0), normalize(vec3(0,1,0)))
    };

    surfaces[0]->material.albedo = Color(255,0,0);
    surfaces[1]->material.albedo = Color(0,255,0);
    surfaces[2]->material.albedo = Color(0,0,255);
    surfaces[3]->material.albedo = Color(50,50,50);

    lights = {
        new Light(vec3(-4, 10, -10), 1.0f),
        new Light(vec3(8, 10, -10), 0.1f)
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
    
    const Color BACKGROUND_COLOR = Color(30,30,30);
    Color color = BACKGROUND_COLOR;

    Surface::HitRecord intersection = closestIntersectionWithSurface(ray);
    // If nothing was hit, return the background color.
    if (intersection.hit == false) {
        return BACKGROUND_COLOR;
    }
    color = intersection.surface->material.albedo * accumulateLight(ray, intersection);
    color.clamp();
    return color;
}

 float Scene::accumulateLight(Ray& ray, Surface::HitRecord& hitRecord) {
    float totalLight = 0.0f;
    vec3 intersectionPosition = hitRecord.position;
    Surface* surface = hitRecord.surface;
    totalLight += 0.2f; // ambient light
    for (Light* light : lights) {

        // Shadow: check if the light is visible from the intersection point
        Ray shadowRay = Ray(intersectionPosition, normalize(light->origin - intersectionPosition));

        // Diffuse!
        // calculate what direction the light is coming from
        vec3 incidentLightDirection = normalize(light->origin - intersectionPosition); //TODO: what should the w be here?
        float diffuse = max(0.0f, dot(hitRecord.normal, incidentLightDirection)); // multiply by constant here
        totalLight += diffuse;
        // Specular!
        // check if the material is specular at all
        if (surface->material.specular > 0.001f) {
            vec3 reflection = reflect(ray.direction, hitRecord.normal);
            float specular = pow(max(0.0f, dot(reflection, incidentLightDirection)), surface->material.specular);
            totalLight += specular;
        }

    }
    return totalLight;
 }

// Check for intersections with all objects
    Surface::HitRecord Scene::closestIntersectionWithSurface(Ray& ray) {
        Surface::HitRecord closestHit = Surface::HitRecord::Miss();
        float closestDistance = INFINITY;
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
        return closestHit;
    }

Scene::~Scene() {
    delete camera;
    for (Surface* surface : surfaces) {
        delete surface;
    }
}