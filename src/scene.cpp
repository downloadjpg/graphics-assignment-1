#include "scene.h"
#include "camera.h"
#include <glm/vec3.hpp>

Scene::Scene() {
    camera = new Camera(
        vec3(0,1,0), // origin
        vec3(0,1,0), // up
        vec3(0,0,-1) // forward
    );
    // Add some shapes!
    surfaces.push_back(new Sphere(vec3(0, 0, -20), 1.0f));
    surfaces.push_back(new Plane(vec3(0, 0, 0), vec3(0, 1, 0)));
}
// Renders an image of the scene with the given width and height. This is the main function of the raytracer.
// Returns a pointer to the image data, which is a 1D array of RGB values.
unsigned char* Scene::renderImage(int width, int height) {
    camera->imageWidth = width;
    camera->imageHeight = height;

    // Generate an HDR buffer, representing the raw amount/color of light at each pixel.

    // NOTE: Radiance is the pure amount of light (for each color), 
    //      while luminosity is how bright said color appears to the human eye.
    vec3* radianceBuffer = new vec3[width * height];
    float maxWhitePoint = writeRadianceBuffer(radianceBuffer, width, height);  // Useful return value
    // actually, setting the white point to the maximum value in the image is not a good idea, it changes every damn frame.
    maxWhitePoint = 1.0f;  // for now, we'll just set it to 1.0f
    // Map this buffer so each value is between 0 and 1. (Reinhard tone mapping).
    applyToneMap(radianceBuffer, width, height, maxWhitePoint);
    // Then, convert the mapped radiance to an RGB value and store it in the image array.
    unsigned char* image = new unsigned char[width * height * 3];
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (j * width + i) * 3;
            vec3 color = radianceBuffer[j * width + i];
            image[index] = (unsigned char)(color.r * 255);
            image[index + 1] = (unsigned char)(color.g * 255);
            image[index + 2] = (unsigned char)(color.b * 255);
        }
    }
    return image;
}


vec3 Scene::colorRay(Ray& ray, int depth) {
    const vec3 BACKGROUND_COLOR = normalize(vec3(0.3f, 0.2f, 0.5f));
    // If we've reached the maximum recursion depth, return black.
    // if (depth > maxDepth) {
    //     return vec3(0, 0, 0);
    // }
    // Find the closest intersection with a surface.
    HitRecord intersection = closestIntersectionWithSurface(ray, surfaces);
    // If we didn't hit anything, return the background color.
    if (!intersection.hit) {
        return BACKGROUND_COLOR;
    }
    // Otherwise, calculate the color of the ray based on the hit.
    return intersection.surface->material.albedo;
}

HitRecord Scene::closestIntersectionWithSurface(Ray &ray, std::vector<Surface *> surfaces)
{
    if (surfaces.size() == 0) {
        return HitRecord::Miss();
    }
    HitRecord closestHit = HitRecord::Miss();
    for (auto surface : surfaces) {
        HitRecord intersection = surface->intersection(ray);

        if (intersection.hit && intersection.distance > tMin && intersection.distance < tMax) {
            if (intersection.distance < closestHit.distance) {
                closestHit = intersection;
            }
        }
    }

    return closestHit;
}

Scene::~Scene() {
    delete camera;
    for (auto surface : surfaces) {
        delete surface;
    }
    for (auto light : lights) {
        delete light;
    }
}

// Returns the luminance of a color
float luminance(vec3 color) {
    return dot(color, vec3(0.2126f, 0.7152f, 0.0722f));
}

float Scene::writeRadianceBuffer(vec3* radianceBuffer, int width, int height) {
    float maxWhitePoint = 0.0f;
    // Loop through every pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Generate a ray for the current pixel
            Ray ray = camera->generateRay(j, i); // j,i is x,y
            // Determine what color the ray ultimately contributes to the pixel
            vec3 c = colorRay(ray, 0);
            radianceBuffer[i * width + j] = c;
            float magnitude = c.length();
            maxWhitePoint = std::max(maxWhitePoint, magnitude);
        }
    }
    return maxWhitePoint;
} 


void Scene::applyToneMap(vec3* buffer, int width, int height, float whitePoint) {
    // https://64.github.io/tonemapping/
    // Extended Reinhard tone mapping
    for (int i = 0; i < width * height; i++) {
        // Calculate the luminance of the color
        float oldLuminance = luminance(buffer[i]);
        // Convert our luminance from [0, inf) to [0, 1]
        float numerator = oldLuminance * (1.0f + (oldLuminance / (whitePoint * whitePoint)));
        float newLuminance = numerator / (1.0f + oldLuminance);
        // Now we adjust adjust the color so it matches this new luminance.
        buffer[i] *= newLuminance / oldLuminance;
    }
}
