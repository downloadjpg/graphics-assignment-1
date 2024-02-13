#include "scene.h"
#include "camera.h"
#include "surface.h"
#include <glm/vec3.hpp>

Scene::Scene() {
    camera = new Camera(
        vec3(0,1,0), // origin
        vec3(0,1,0), // up
        vec3(0,0,-1) // forward
    );

    renderer = new Renderer(this);
    // Add some shapes!
    surfaces.push_back(new Sphere(vec3(0, 0, -20), 1.0f));
    surfaces.push_back(new Sphere(vec3(4, 2, -10), 1.0f));
    surfaces.push_back(new Plane(vec3(0, 0, 0), vec3(0, 1, 0)));
    surfaces.push_back(new Tetrahedron(vec3(-4, 4, -10), 2.0f));

    surfaces[0]->material.albedo = vec3(0.7f, 0.1f, 0.2f); // pink
    surfaces[1]->material.albedo = vec3(0.2f, 0.7f, 0.1f); // green
    surfaces[1]->material.phongExponent = 150.0f;
    surfaces[2]->material.reflectivity = 0.5f;
    // Let there be light!
    lights.push_back(Light::newAmbientLight(vec3(0, 0, 0), vec3(1, 1, 1), 0.2f));
    lights.push_back(Light::newDirectionalLight(vec3(1, -1, -1), vec3(1, 1, 1), 0.4f));
    lights.push_back(Light::newPointLight(vec3(0, 5, -10), vec3(1, 1, 1), 0.4f));
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
// float luminance(vec3 color) {
//     return dot(color, vec3(0.2126f, 0.7152f, 0.0722f));
// }

// float Scene::writeRadianceBuffer(vec3* radianceBuffer, int width, int height) {
//     float maxWhitePoint = 0.0f;
//     // Loop through every pixel in the image
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             // Generate a ray for the current pixel
//             Ray ray = camera->generateRay(j, i); // j,i is x,y
//             // Determine what color the ray ultimately contributes to the pixel
//             vec3 c = colorRay(ray, 0);
//             radianceBuffer[i * width + j] = c;
//             float magnitude = c.length();
//             maxWhitePoint = std::max(maxWhitePoint, magnitude);
//         }
//     }
//     return maxWhitePoint;
// } 


// void Scene::applyToneMap(vec3* buffer, int width, int height, float whitePoint) {
//     // https://64.github.io/tonemapping/
//     // Extended Reinhard tone mapping
//     for (int i = 0; i < width * height; i++) {
//         // Calculate the luminance of the color
//         float oldLuminance = luminance(buffer[i]);
//         // Convert our luminance from [0, inf) to [0, 1]
//         float numerator = oldLuminance * (1.0f + (oldLuminance / (whitePoint * whitePoint)));
//         float newLuminance = numerator / (1.0f + oldLuminance);
//         // Now we adjust adjust the color so it matches this new luminance.
//         buffer[i] *= newLuminance / oldLuminance;
//     }
// }
