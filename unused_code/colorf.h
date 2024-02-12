#pragma once
#include <cmath>

struct ColorF {
    /*
    * A color is represented by three floats, each between 0 and 1.
    * Mixing colors should be done by adding them together, then normalizing the result.
    */
    float r, g, b;

    ColorF() : r(0), g(0), b(0) {} // Default constructor
    ColorF(float r, float g, float b) : r(r), g(g), b(b) {}

    ColorF Mix(const ColorF &a, const ColorF &b) {
        ColorF n = ColorF(
            a.r + b.r,
            a.g + b.g,
            a.b + b.b);
        float magnitude = std::sqrt(n.r * n.r + n.g * n.g + n.b * n.b);
        return ColorF(n.r / magnitude, n.g / magnitude, n.b / magnitude);
    }

};

// When adding light, we take a weighted average of the two colors, where the weights are the light intensities.
struct ColoredLight {
    ColorF color;
    float intensity;

    ColoredLight() : color(ColorF(0, 0, 0)), intensity(0) {} // Default constructor
    ColoredLight(ColorF color, float intensity) : color(color), intensity(intensity) {}

    // Additive mixing of light!
    ColoredLight Mix(const ColoredLight &a, const ColoredLight &b) {
        float totalIntensity = a.intensity + b.intensity;
        ColorF nColor = ColorF(
            (a.color.r * a.intensity + b.color.r * b.intensity) / totalIntensity,
            (a.color.g * a.intensity + b.color.g * b.intensity) / totalIntensity,
            (a.color.b * a.intensity + b.color.b * b.intensity) / totalIntensity
        );
        return ColoredLight(nColor, totalIntensity);
    }
};