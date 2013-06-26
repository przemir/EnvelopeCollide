/* 
 * File:   opengl.h
 * Author: Przemysław Bągard
 *
 * Created on 14 wrzesień 2010, 17:33
 */

#ifndef GEOMETRY_OPENGL_H
#define	GEOMETRY_OPENGL_H

#include <GL/gl.h>
#include "Point.h"
#include "Point2D.h"

/// Functions to display this geometry structures using openGL
namespace EnvelopeGeometry{

/// returns the coordinates of a point that is displayed in viewport on the coordinates (x,y)
/// ((0,0) is left up corner)
Point clickedPoint(int x, int y);

/// intended to be used between the glBegin and glEnd procedures - apply openGL functions for given parameters
void renderVertex(const Point& pos, const fPoint& tex);

/// intended to be used between the glBegin and glEnd procedures - apply openGL functions for given parameters
void renderVertex(const Point& pos);

/// intended to be used between the glBegin and glEnd procedures - apply openGL functions for given parameters
void renderVertex(const iPoint& pos, const fPoint& tex);

/// intended to be used between the glBegin and glEnd procedures - apply openGL functions for given parameters
void renderVertex(const iPoint& pos);

/// displays point (default size value is 1.0f)
void renderPoint(const Point& A, const float& size);

/// displays line segment (default size value is 1.0f)
void renderLine(const Point& A, const Point& B, const float& size);

/// displays triangle (without texture)
void renderTriangle(const Point& A, const Point& B, const Point& C);

/// displays triangle edges (default size value is 1.0f)
void renderTriangleLine(const Point& A, const Point& B, const Point& C, const float& size);

/// displays triangle (with texture)
void renderTriangle(const Point& A, const Point& B, const Point& C, const fPoint& texA, const fPoint& texB, const fPoint& texC);

/// displays 2D point (integer coordinates) (default size value is 1.0f)
void renderPoint(const iPoint& A, const float& size);

/// displays 2D line segment (integer coordinates) (default size value is 1.0f)
void renderLine(const iPoint& A, const iPoint& B, const float& size);

/// displays 2D triangle (integer coordinates) (without texture)
void renderTriangle(const iPoint& A, const iPoint& B, const iPoint& C);

/// displays 2D triangle edges (integer coordinates) (default size value is 1.0f)
void renderTriangleLine(const iPoint& A, const iPoint& B, const iPoint& C, const float& size);

/// displays 2D triangle (integer coordinates) (with texture)
void renderTriangle(const iPoint& A, const iPoint& B, const iPoint& C, const fPoint& texA, const fPoint& texB, const fPoint& texC);

/// displays 2D point (float coordinates) (default size value is 1.0f)
void renderPoint(const fPoint& A, const float& size);

/// displays 2D line segment (float coordinates) (default size value is 1.0f)
void renderLine(const fPoint& A, const fPoint& B, const float& size);

/// displays 2D triangle (float coordinates) (without texture)
void renderTriangle(const fPoint& A, const fPoint& B, const fPoint& C);

/// displays 2D triangle edges (float coordinates) (default size value is 1.0f)
void renderTriangleLine(const fPoint& A, const fPoint& B, const fPoint& C, const float& size);

/// displays 2D triangle (float coordinates) (with texture)
void renderTriangle(const fPoint& A, const fPoint& B, const fPoint& C, const fPoint& texA, const fPoint& texB, const fPoint& texC);


}

/******************************* implementation *******************************/

namespace EnvelopeGeometry {

inline void renderVertex(const Point& pos, const fPoint& tex) { glTexCoord2f(tex.x, tex.y); glVertex3f(pos.x, pos.y, pos.z); }
inline void renderVertex(const Point& pos) { glVertex3f(pos.x, pos.y, pos.z); }
inline void renderVertex(const iPoint& pos, const fPoint& tex) { glTexCoord2f(tex.x, tex.y); glVertex2i(pos.x, pos.y); }
inline void renderVertex(const iPoint& pos) { glVertex2i(pos.x, pos.y); }
//////////////////////////////////////////
inline void renderPoint(const Point& A, const float& size = 1.0f)
{
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex3f(A.x, A.y, A.z);
	glEnd();
	glPointSize(1.0f);
}
inline void renderLine(const Point& A, const Point& B, const float& size = 1.0f)
{
	glLineWidth(size);
	glBegin(GL_LINES);
		glVertex3f(A.x, A.y, A.z);
		glVertex3f(B.x, B.y, B.z);
	glEnd();
	glLineWidth(1.0f);
}
inline void renderTriangle(const Point& A, const Point& B, const Point& C)
{
	glBegin(GL_TRIANGLES);
		glVertex3f(A.x, A.y, A.z);
		glVertex3f(B.x, B.y, B.z);
		glVertex3f(C.x, C.y, C.z);
	glEnd();
}
inline void renderTriangleLine(const Point& A, const Point& B, const Point& C, const float& size = 1.0f)
{
    renderLine(A, B, size);
    renderLine(B, C, size);
    renderLine(C, A, size);
}
inline void renderTriangle(const Point& A, const Point& B, const Point& C, const fPoint& texA, const fPoint& texB, const fPoint& texC)
{
	glBegin(GL_TRIANGLES);
		glTexCoord2f(texA.x, texA.y); glVertex3f(A.x, A.y, A.z);
		glTexCoord2f(texB.x, texB.y); glVertex3f(B.x, B.y, B.z);
		glTexCoord2f(texC.x, texC.y); glVertex3f(C.x, C.y, C.z);
	glEnd();
}
//////////////////////////////////////////
inline void renderPoint(const iPoint& A, const float& size = 1.0f)
{
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex2i(A.x, A.y);
	glEnd();
	glPointSize(1.0f);
}
inline void renderLine(const iPoint& A, const iPoint& B, const float& size = 1.0f)
{
	glLineWidth(size);
	glBegin(GL_LINES);
		glVertex2i(A.x, A.y);
		glVertex2i(B.x, B.y);
	glEnd();
	glLineWidth(1.0f);
}
inline void renderTriangle(const iPoint& A, const iPoint& B, const iPoint& C)
{
	glBegin(GL_TRIANGLES);
		glVertex2i(A.x, A.y);
		glVertex2i(B.x, B.y);
		glVertex2i(C.x, C.y);
	glEnd();
}
inline void renderTriangleLine(const iPoint& A, const iPoint& B, const iPoint& C, const float& size = 1.0f)
{
    renderLine(A, B, size);
    renderLine(B, C, size);
    renderLine(C, A, size);
}
inline void renderTriangle(const iPoint& A, const iPoint& B, const iPoint& C, const fPoint& texA, const fPoint& texB, const fPoint& texC)
{
	glBegin(GL_TRIANGLES);
		glTexCoord2f(texA.x, texA.y); glVertex2i(A.x, A.y);
		glTexCoord2f(texB.x, texB.y); glVertex2i(B.x, B.y);
		glTexCoord2f(texC.x, texC.y); glVertex2i(C.x, C.y);
	glEnd();
}
/////////////////////////
inline void renderPoint(const fPoint& A, const float& size = 1.0f)
{
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex2f(A.x, A.y);
	glEnd();
	glPointSize(1.0f);
}
inline void renderLine(const fPoint& A, const fPoint& B, const float& size = 1.0f)
{
	glLineWidth(size);
	glBegin(GL_LINES);
		glVertex2f(A.x, A.y);
		glVertex2f(B.x, B.y);
	glEnd();
	glLineWidth(1.0f);
}
inline void renderTriangle(const fPoint& A, const fPoint& B, const fPoint& C)
{
	glBegin(GL_TRIANGLES);
		glVertex2f(A.x, A.y);
		glVertex2f(B.x, B.y);
		glVertex2f(C.x, C.y);
	glEnd();
}
inline void renderTriangleLine(const fPoint& A, const fPoint& B, const fPoint& C, const float& size = 1.0f)
{
    renderLine(A, B, size);
    renderLine(B, C, size);
    renderLine(C, A, size);
}
inline void renderTriangle(const fPoint& A, const fPoint& B, const fPoint& C, const fPoint& texA, const fPoint& texB, const fPoint& texC)
{
	glBegin(GL_TRIANGLES);
		glTexCoord2f(texA.x, texA.y); glVertex2f(A.x, A.y);
		glTexCoord2f(texB.x, texB.y); glVertex2f(B.x, B.y);
		glTexCoord2f(texC.x, texC.y); glVertex2f(C.x, C.y);
	glEnd();
}

}

#endif	/* GEOMETRY_OPENGL_H */
