#include "renderer.h"
#include "scene.h"
#include "surface.h"
#include "camera.h"
#include "ray.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <vector>
#include <limits>


Renderer::Renderer(Scene* scene) {
    this->scene = scene;
    this->camera = scene->camera;

    outputBuffer = new unsigned char[width * height * 3];
    radianceBuffer = new vec3[width * height];

}

Renderer::~Renderer() {
    if (outputBuffer != nullptr)
        delete[] outputBuffer;
    if (radianceBuffer != nullptr)
        delete[] outputBuffer;
}

// Returns a pointer to the image data, which is a 1D array of RGB values.
unsigned char* Renderer::renderImage() {
    writeOutputBuffer();
    return outputBuffer;
}

// ==============================


vec3 Renderer::colorRay(Ray& ray, int depth) {

    const float ambientWeight = 0.5f;
    const float diffuseWeight = 0.5f;
    const float specularWeight = 0.8f;
    const bool shadowsEnabled = true;

    const vec3 BACKGROUND_COLOR = vec3(0.2f, 0.1f, 0.6f); // ambient light is typically 0.2f
    // If we've reached the maximum recursion depth, return black.
     if (depth > maxDepth) {
         return vec3(0, 0, 0);
    }
    // Find the closest intersection with a surface.
    HitRecord intersection = closestIntersectionWithSurface(ray);
    // If we didn't hit anything, return the background color.
    if (!intersection.hit) {
        return BACKGROUND_COLOR;
    }
    // Otherwise, calculate the color of the ray based on the hit.
    if (scene->lights.size() == 0) { // no lights
        return intersection.surface->material.albedo;
    }
    
    vec3 color = vec3(0, 0, 0);

    for (auto light : scene->lights) {
        if (light->type == Light::Type::AMBIENT) {
            color += intersection.surface->material.albedo * light->color * light->intensity * ambientWeight;
            continue;
        }
        
        vec3 lightDir = light->directionToLight(intersection.position);
        float lightDist = light->distanceToLight(intersection.position);

        // Shadows!
        if (shadowsEnabled) {
            Ray shadowRay = Ray(intersection.position, lightDir);
            HitRecord shadowIntersection = closestIntersectionWithSurface(shadowRay, std::numeric_limits<float>::epsilon(), lightDist);
            if (shadowIntersection.hit) {
                continue;
            }
        }

        // Diffuse lighting
        vec3 N = intersection.normal;
        vec3 V = ray.direction;
        float diffuse = std::max(0.0f, dot(N, lightDir));
        diffuse *= diffuseWeight;
        color += diffuse * intersection.surface->material.albedo * light->color * light->intensity;

        // Reflections!
        if (intersection.surface->material.reflectivity > 0.0f) {
            vec3 reflectionDir = glm::reflect(ray.direction, intersection.normal);
            Ray reflectionRay = Ray(intersection.position, reflectionDir);
            vec3 reflectionColor = colorRay(reflectionRay, depth + 1);
            color += reflectionColor * intersection.surface->material.albedo * intersection.surface->material.reflectivity;
        }

        // Specular lighting
        if (intersection.surface->material.phongExponent < 0) {
            continue;
        }
        vec3 R = glm::normalize(glm::reflect(V, N));
        float specular = std::pow(std::max(0.0f, dot(R, lightDir)), intersection.surface->material.phongExponent);
        specular *= specularWeight;
        color += specular * light->color * light->intensity;

    }
    return color;
}

HitRecord Renderer::closestIntersectionWithSurface(Ray &ray, float tMin, float tMax) {
    if (scene->surfaces.size() == 0) {
        return HitRecord::Miss();
    }
    HitRecord closestHit = HitRecord::Miss();
    for (auto surface : scene->surfaces) {
        HitRecord intersection = surface->intersection(ray);

        if (intersection.hit && intersection.distance > tMin && intersection.distance < tMax) {
            if (intersection.distance < closestHit.distance) {
                closestHit = intersection;
            }
        }
    }
    return closestHit;
}


// Renders an image of the scene with the given width and height. This is the main function of the raytracer!!
void Renderer::writeRadianceBuffer() {
    //float maxWhitePoint = 0.0f;
    // Loop through every pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Generate a ray for the current pixel
            Ray ray = camera->generateRay(j, i);
            // Determine what color the ray ultimately contributes to the pixel
            vec3 c = colorRay(ray, 0);
            radianceBuffer[i * width + j] = c;
            //float magnitude = c.length();
            // maxWhitePoint = std::max(maxWhitePoint, magnitude);
        }
    }
    // return maxWhitePoint;
} 

void Renderer::writeOutputBuffer() {
    writeRadianceBuffer(); // image where each pixel has 3 [0, inf] values
    //scene->applyToneMap(radianceBuffer, width, height, maxWhitePoint);
    //radianceBufferToImage();
    // Convert the radiance buffer to an image
    for (int i = 0; i < width * height; i++) {
        vec3 color = radianceBuffer[i];
        color.r = std::min(color.r, 1.0f);
        color.g = std::min(color.g, 1.0f);
        color.b = std::min(color.b, 1.0f);
        
        // Convert the color to an 8-bit RGB value
        outputBuffer[i * 3 + 0] = (unsigned char)(255.0f * color.r);
        outputBuffer[i * 3 + 1] = (unsigned char)(255.0f * color.g);
        outputBuffer[i * 3 + 2] = (unsigned char)(255.0f * color.b);
    }
}
/*

// Returns the luminance of a color
float luminance(vec3 color) {
    return dot(color, vec3(0.2126f, 0.7152f, 0.0722f));
}
*/

/*
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
}*/
