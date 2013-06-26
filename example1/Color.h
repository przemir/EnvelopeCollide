#ifndef COLOR_H
#define COLOR_H

/// Color
struct Color
{
    Color();
    Color(float r, float g, float b);

    /// red, green, blue
    float r, g, b;

    /// Use 'glColor4f' for this struct.
    void use() const;
};

#endif // COLOR_H
