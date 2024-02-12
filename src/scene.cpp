#include "scene.h"
#include "camera.h"

Scene::Scene() {
    camera = new Camera(
        vec3(0,2,0), // origin
        vec3(0,1,0), // up
        vec3(0,0,-1) // forward
    );
    // Add some shapes!
}
// Renders an image of the scene with the given width and height. This is the main function of the raytracer.
// Returns a pointer to the image data, which is a 1D array of RGB values.
unsigned char* Scene::renderImage(int width, int height) {
    camera->imageWidth = width;
    camera->imageHeight = height;

    // Generate an HDR buffer, representing the raw amount/color of light at each pixel.

    // NOTE: Radiance is the pure amount of light (for each color), 
    // while luminosity is how bright said color appears to the human eye.
    vec3* radianceBuffer = new vec3[width * height];
    // The function also returns the maximum white point in the image, which is used for tone mapping.
    float maxWhitePoint = writeRadianceBuffer(radianceBuffer, width, height);  
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
    // If we've reached the maximum recursion depth, return black.
    if (depth > maxDepth) {
        return vec3(0, 0, 0);
    }
    // Find the closest intersection with a surface.
    HitRecord hit = closestIntersectionWithSurface(ray, surfaces);
    // If we didn't hit anything, return the background color.
    if (!hit.hit) {
        return vec3(0, 0, 0);
    }
    // Otherwise, calculate the color of the ray based on the hit.
    return hit.material.albedo;
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
            maxWhitePoint = std::max(maxWhitePoint, magnitude); // TODO: this cast might be stupid
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
