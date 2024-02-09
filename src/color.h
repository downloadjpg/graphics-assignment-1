#pragma once

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;

    // Default constructor
    Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {};

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

    // Clamp color values to 0-255
    void clamp() {
        r = r > 255 ? 255 : r;
        g = g > 255 ? 255 : g;
        b = b > 255 ? 255 : b;

        r = r < 0 ? 0 : r;
        g = g < 0 ? 0 : g;
        b = b < 0 ? 0 : b;
    }
};