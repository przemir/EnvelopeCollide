/* 
 * File:   shapes.h
 * Author: Przemek
 *
 * Created on 4 październik 2010, 17:07
 */

#ifndef SHAPES_H
#define	SHAPES_H

namespace EnvelopeGeometry {

struct Point;

/// shape display mode
enum DRAW_TYPE {
    /// display lines
    DRAW_TYPE_LINES = 0,
    /// display filled shapes
    DRAW_TYPE_FILLED = 1
};

/// displays sphere with: 'pos' - position of sphere, 'r' - range, 'k' - quantity of sections, 'n' - quantity of vertices in each section, 'type' - display mode
void drawSphere(Point pos, int k, int n, float r, DRAW_TYPE type);

/// displays envelope of segment line with: 'AB' - segment line, 'r' - range, 'k' - quantity of sections, 'n' - quantity of vertices in each section, 'type' - display mode
void drawQuasiLine(Point A, Point B, int k, int n, float r, DRAW_TYPE type);

/// displays envelope of triangle with: 'ABC' - triangle, 'r' - range, 'k' - quantity of sections, 'n' - quantity of vertices in each section, 'type' - display mode
void drawQuasiTriangle(Point A, Point B, Point C, int n, int k, float r, DRAW_TYPE type);

}

#endif	/* SHAPES_H */

