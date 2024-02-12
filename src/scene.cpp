#include "scene.h"
#include <iostream>
/*
* TODOLIST:
* - [x] Change perspective with button
* - [x] Directional/Ambient/Point light system
* - [x] Triangle
* - [?] Tonemapping
* - [?] Fix lighting?
* - [ ] "Glossy" surface
* - [ ] Tetrahedron

* - [ ] Camera rotation
* - [ ] Refactor to use smart pointers
* - [ ] Export video somehow??
* - [ ] Paper report
* - [ ] Phong shading??/

*/
Scene::Scene() {

    camera = new Camera(
        vec3(0,1,0), // origin
        vec3(0,1,0), // up
        vec3(0,0,-1) // forward
    );

    surfaces = {
        new Sphere(vec3(1,0.5f,-5), 0.2f),
        new Sphere(vec3(0,0.5f,-8), 0.2f),
        new Sphere(vec3(-1,0.8f,-5), 0.4f),
        new Plane(vec3(0,0,0), normalize(vec3(0,1,0))),
        //new Tetrahedron(vec3(1,-4,0), 0.3f)
        new Triangle(vec3(0.0,-0.2,0.0), vec3(1,0,0), vec3(0,1,0))
    };

    surfaces[0]->material.albedo = ColorF(1,0,0);
    surfaces[1]->material.albedo = ColorF(0,1,0);
    surfaces[2]->material.albedo = ColorF(0,0,1);
    surfaces[3]->material.albedo = ColorF(0.2,0.4,0.3);

    surfaces[0]->material.specular = 2.0f;
    surfaces[1]->material.specular = 50.0f;
    surfaces[2]->material.specular = 80.0f;

    lights = {
        Light::NewAmbientLight(1.0f),
       // Light::NewPointLight(vec3(0, 2, -5), 0.6f),
        Light::NewDirectionalLight(normalize(vec3(0.3, -1, 0.3)), 2.0f),
        Light::NewDirectionalLight(normalize(vec3(-0.3, -1, -0.3)), 2.0f)
    };
}


unsigned char* Scene::renderImage(const int width, const int height) {
    // Image array to be returned to the actual rasterizer.
    unsigned char* image = new unsigned char[width * height * 3];

    // actually put this in a luminance buffer or something
    ColorF luminanceBuffer[width * height];
    float averageLuminance;

    // Loop through every pixel in the image
    for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

        // Generate a ray for the current pixel
        Ray ray = camera->generateRay(j, i); // j,i is x,y
        // Determine what color the ray ultimately contributes to the pixel
        ColorF c = rayColor(ray);
        averageLuminance += c.magnitude();
        luminanceBuffer[i * width + j] = c;
    }}
    // divide by average, then put through a little curve thing
    averageLuminance /= (width * height);

    for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

        const float gamma = 20.0f;
        int idLum = i * width + j;
        float lum = luminanceBuffer[idLum].magnitude();
        float scaledLuminance = lum / averageLuminance / 9.6;
        scaledLuminance *= gamma;
        float displayLuminance = scaledLuminance / (1 + scaledLuminance);

        float finalFinalFinalMultiplier = displayLuminance/lum;
        if (finalFinalFinalMultiplier < 0.001f || finalFinalFinalMultiplier > 1.0f)  std::cout << finalFinalFinalMultiplier << std::endl;
        Color c = Color(
            luminanceBuffer[idLum].r * 255.0f * finalFinalFinalMultiplier,
            luminanceBuffer[idLum].g * 255.0f * finalFinalFinalMultiplier, 
            luminanceBuffer[idLum].b * 255.0f * finalFinalFinalMultiplier
        );

        int idx = (i * width + j) * 3;
        image[idx] = c.r;
        image[idx+1] = c.g;
        image[idx+2] = c.b;
    }}
    return image;
}


ColorF Scene::rayColor(Ray& ray) {
    
    const ColorF BACKGROUND_COLOR = ColorF(0.3f, 0.2f, 0.5f);
    ColorF color = BACKGROUND_COLOR;

    Surface::HitRecord intersection = closestIntersectionWithSurface(ray);
    // If nothing was hit, return the background color.
    if (intersection.hit == false) {
        return BACKGROUND_COLOR;
    }
    float l = accumulateLight(ray, intersection);
    color = intersection.surface->material.albedo * l;// accumulateLight(ray, intersection);
    return color;
}

 float Scene::accumulateLight(Ray& ray, Surface::HitRecord& hitRecord) {
    const float diffuseWeight = 5.0f;
    const float specularWeight = 20.0f;

    float totalLight = 0.0f;
    vec3 intersectionPosition = hitRecord.position;
    Surface* surface = hitRecord.surface;
    for (Light* light : lights) {

        if (light->type == Light::Type::AMBIENT) {
            totalLight += light->intensity;
            continue;
        }

        // calculate what direction the light is coming from
        vec3 incidentLightDirection;
        if (light->type == Light::Type::DIRECTIONAL) 
            incidentLightDirection = light->direction;

        else if (light->type == Light::Type::POINT)
            incidentLightDirection = normalize(intersectionPosition - light->origin);

        // Shadows! Check if the light is visible from the intersection point
        Ray shadowRay = Ray(intersectionPosition, -incidentLightDirection);
        if (closestIntersectionWithSurface(shadowRay).hit)   
            continue;

        // Diffuse!
        float a = max(0.0f, dot(hitRecord.normal, -incidentLightDirection));
        float diffuse = a * diffuseWeight * light->intensity;
        totalLight += diffuse;

        // Specular!
        // check if the material is specular at all
        if (surface->material.specular > 0.001f) {
            vec3 reflection = normalize(reflect(ray.direction, hitRecord.normal));
            float b = pow(max(0.0f, dot(reflection, -incidentLightDirection)), surface->material.specular);
            float specular = b * specularWeight * light->intensity;
            totalLight += specular;
        }


        // Reflective surface
        // if reflective...
        // create a depth variable
        // set it to 0, increment it for every recursive call. when you hit a
        // maximum depth, stop recursively calling.
        // tbh a max_depth of 1 is probably fine. (only recursively calls once)
        //
        // then recursively call this function with a new ray emanating from the
        // intersection point in the direction of the reflection of the original ray
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
            if (intersection.distance < 0.0000001f) {continue;} // ignore intersections behind the ray's origin (or very close to it, to avoid self-intersection artifacts
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


/*
    https://bruop.github.io/exposure/
    The Tonemapping pipeline has 5 stages:

    Render your scene into a framebuffer that supports HDR values i.e. a floating point buffer with 16-bits or 32-bits per channel. Which one you choose is (as always) a tradeoff between precision, range and memory, but for my scene I’m going to go with a RGBA16F framebuffer. Make sure your output is in linear space.
    Using either mip mapping or compute shaders, find the average scene luminance using the HDR color buffer.

    In a separate render pass, produce a linear, clamped color buffer by:
        Scaling the input using the average scene luminance to obtain a “calibrated” RGB value
        Scaling the calibrated value using a Tone Curve, inputting either the RGB or the luminance value of the fragment

    Transform your linear clamped value using the inverse electrical optical transfer function (EOTF).
        With typical, non-HDR displays, we’re basically transforming our linear color into sRGB color space by applying the inverse gamma transformation
    Write this non-linear result to the back buffer, which is presented to the user.
*/