#include "scene.h"
#include "camera.h"
#include <glm/vec3.hpp>


/* TODO:
* - [ ] Report
* - [ ] Export to video 
* - [ ] Add triangle/tetrahedra shapes
* - [ ] Add more lights
* - [ ] Color lights
*/

Scene::Scene() {
    camera = new Camera(
        vec3(0,1,0), // origin
        vec3(0,1,0), // up
        vec3(0,0,-1) // forward
    );
    // Add some shapes!
    surfaces.push_back(new Sphere(vec3(0, 0, -10), 0.5f));
    surfaces.push_back(new Sphere(vec3(-1.5, 0.8f, -10), 0.25f));
    surfaces.push_back(new Tetrahedron(vec3(1.5, 0.8f, -10), 0.5f));
    //surfaces.push_back(new Triangle(vec3(-1, 1, -15), vec3(1, 1, -15), vec3(0, 0, -15)));
    surfaces.push_back(new Plane(vec3(0, 0, 0), vec3(0, 1, 0)));
    surfaces[2]->material.reflective = 0.7f;
    surfaces[1]->material.specular = 15.0f;

    surfaces[0]->material.albedo = vec3(0.3f, 0.8f, 0.2f);

    // Let there be light!
    lights.push_back(Light::newAmbientLight(vec3(1, 1, 1), 0.3f));
    lights.push_back(Light::newDirectionalLight(vec3(-1, -1, -1), vec3(1,1,1), 0.90f)); // From the top left
    //lights.push_back(Light::newPointLight(vec3(0, 5, -5), vec3(1,1,1), 0.20f));    // Situated a little above and in front of the spheres
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
    // Map this buffer so each value is between 0 and 1. (Reinhard tone mapping).
    //applyToneMap(radianceBuffer, width, height, maxWhitePoint);
    bruteForceClamp(radianceBuffer, width, height);
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
    const vec3 BACKGROUND_COLOR = vec3(0.3f, 0.2f, 0.5f);
    const int maxDepth = 2;
    //If we've reached the maximum recursion depth, return black.
    if (depth > maxDepth) {
         return vec3(0, 0, 0);
     }
    // Find the closest intersection with a surface.
    HitRecord intersection = closestIntersectionWithSurface(ray, surfaces);
    // If we didn't hit anything, return the background color.
    if (!intersection.hit) {
        return BACKGROUND_COLOR;
    }
    // Otherwise, calculate the color of the ray based on the hit.
    vec3 color = vec3(0, 0, 0);
    vec3& albedo = intersection.surface->material.albedo;
    
    // Reflections!
    if (intersection.surface->material.reflective > 0) {
        vec3 reflection = reflect(ray.direction, intersection.normal);
        Ray reflectedRay = Ray(
            intersection.position + reflection * EPSILON,
            reflection
        );
        color += colorRay(reflectedRay, depth + 1) * albedo * intersection.surface->material.reflective;
    }
    for (auto light : lights) {

        // Ambient lighting
        if (light->type == Light::Type::AMBIENT) {
            color += light->color * light->intensity * albedo;
            continue;
        }

        // If the light isn't ambient, then we need to calculate the direction and distance to the light.
        vec3 lightDirection; // intersection TO light
        float lightDistance;

        if (light->type == Light::Type::POINT) {
            lightDirection = normalize(light->origin - intersection.position); // intersection TO light
            lightDistance = (light->origin - intersection.position).length();
        }

        else if (light->type == Light::Type::DIRECTIONAL) {
            lightDirection = -light->direction;
            lightDistance = std::numeric_limits<float>::max();
        }
        // Shadows!
        Ray shadowRay = Ray(
            intersection.position,
            lightDirection
        );
        HitRecord shadowIntersection = closestIntersectionWithSurface(shadowRay, surfaces);
        if (shadowIntersection.hit && shadowIntersection.distance < lightDistance) { continue; }

        // Diffuse
        float diffuse = std::max(dot(intersection.normal, lightDirection), 0.0f);
        color += light->color * albedo * light->intensity * diffuse;

        // Specular lighting
        if (intersection.surface->material.specular > 0) {
            vec3 reflection = reflect(ray.direction, intersection.normal);
            float specular = pow(std::max(dot(reflection, lightDirection), 0.0f), intersection.surface->material.specular);
            color += light->color * light->intensity * specular;
        }


    }

    return color;
}

HitRecord Scene::closestIntersectionWithSurface(Ray &ray, std::vector<Surface*> surfaces)
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



// Returns the luminance of a color
float luminance(vec3 color) {
    return dot(color, vec3(1,1,1));
    return dot(color, vec3(0.2126f, 0.7152f, 0.0722f));
}

float Scene::writeRadianceBuffer(vec3* radianceBuffer, int width, int height) {
    float maxWhitePoint = 0.01f;
    // Loop through every pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Generate a ray for the current pixel
            Ray ray = camera->generateRay(j, i); // j,i is x,y
            // Determine what color the ray ultimately contributes to the pixel
            vec3 c = colorRay(ray, 0);
            radianceBuffer[i * width + j] = c;
            float magnitude = luminance(c);
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
        buffer[i] *= (newLuminance / oldLuminance);
    }
}

void Scene::bruteForceClamp(vec3* buffer, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        // buffer[i].r = std::min(buffer[i].r, 1.0f);
        // buffer[i].g = std::min(buffer[i].g, 1.0f);
        // buffer[i].b = std::min(buffer[i].b, 1.0f);
        buffer[i].r = buffer[i].r / (buffer[i].r + 1.0f);
        buffer[i].g = buffer[i].g / (buffer[i].g + 1.0f);
        buffer[i].b = buffer[i].b / (buffer[i].b + 1.0f);
    }
}
