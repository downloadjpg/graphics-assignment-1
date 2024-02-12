#include <iostream>
#include <cmath>
#pragma once

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;

    // Default constructor
    Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {};

    // Clamp color values to 0-255
    Color clamp() {
        if (r > 255 || b < 0)
            //std::cout << r << std::endl;
        if (g > 255 || b < 0)
           // std::cout << r << std::endl;
        if (b > 255 || b < 0)
           // std::cout << r << std::endl;
        r = r > 255 ? 255 : r;
        g = g > 255 ? 255 : g;
        b = b > 255 ? 255 : b;

        r = r < 0 ? 0 : r;
        g = g < 0 ? 0 : g;
        b = b < 0 ? 0 : b;

        return *this;
    }

    //Multiply by scalar
    Color operator*(float scalar) {
        return Color(
            r * scalar,
            g * scalar,
            b * scalar
        );
    }

    // Add two colors
    Color operator+(Color& other) {
        return Color(
            r + other.r,
            g + other.g,
            b + other.b
        );
    }

};

struct ColorF { // color on a 0 to 1 (or more?) scale! why? idk.
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    // Default constructor
    ColorF(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {};
    ColorF() = default;

    // Clamp color values to 0-1
    ColorF clamp() {
        if (r > 1.0f || b < 0.0f)
            //std::cout << r << std::endl;
        if (g > 1.0f || b < 0.0f)
            //std::cout << r << std::endl;
        if (b > 1.0f || b < 0.0f)
           // std::cout << r << std::endl;
        r = r > 1.0f ? 1.0f : r;
        g = g > 1.0f ? 1.0f : g;
        b = b > 1.0f ? 1.0f: b;

        r = r < 0.0f ? 0.0f : r;
        g = g < 0.0f ? 0.0f : g;
        b = b < 0.0f ? 0.0f : b;
        return *this;
    }

    //Multiply by scalar
    ColorF operator*(float scalar) {
        return ColorF(
            r * scalar,
            g * scalar,
            b * scalar
        );
    }

    // Add two colors
    ColorF operator+(Color& other) {
        return ColorF(
            r + other.r,
            g + other.g,
            b + other.b
        );
    }
    
    float magnitude() {
        return std::sqrt(r*r + g*g + b*b);
    }

};