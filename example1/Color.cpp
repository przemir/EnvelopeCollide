#include "Color.h"
#include "GL/gl.h"

Color::Color() : r(1.0f), g(1.0f), b(1.0f)
{
}

Color::Color(float r, float g, float b) : r(r), g(g), b(b)
{
}

void Color::use() const
{
    glColor4f(r, g, b, 1.0f);
}
