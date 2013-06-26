#include <vector>
//#include "Point.h"
#include "opengl.h"
#include "Plane.h"
#include "shapes.h"
#include "Projector.h"

namespace EnvelopeGeometry {

void draw(std::vector<std::vector <PKT> >& v, DRAW_TYPE type)
{
    int k = v.size();
    int n = v[0].size();
    if(type == DRAW_TYPE_LINES)
    {
        glBegin(GL_LINES);
        for(int l2 = 0; l2 < k; l2++)
        {
            for(int l1 = 0; l1 < n; l1++)
            {
                renderVertex(v[l2][l1]);
                renderVertex(v[l2][(l1+1)%n]);
                if(l2 < k-1)
                {
                    renderVertex(v[l2][l1]);
                    renderVertex(v[l2+1][l1]);
                }
            }
        }
        glEnd();
    }
    if(type == DRAW_TYPE_FILLED)
    {
        glBegin(GL_TRIANGLES);
        for(int l2 = 0; l2 < k; l2++)
        {
            for(int l1 = 0; l1 < n; l1++)
            {
                if(l2 < k-1)
                {
                    renderVertex(v[l2][l1]);
                    renderVertex(v[l2][(l1+1)%n]);
                    renderVertex(v[l2+1][l1]);

                    renderVertex(v[l2][(l1+1)%n]);
                    renderVertex(v[l2+1][(l1+1)%n]);
                    renderVertex(v[l2+1][l1]);
                }
            }
        }
        glEnd();
    }
}

void drawSphere(Point pos, int k, int n, float r, DRAW_TYPE type)
{
    std::vector<std::vector <PKT> > v;  // zewnętrzna lista to poziomica
    float step = (M_PI*2.0f) / (float)n;
    float stepy = M_PI / ((float)k-1);
    for(int l2 = 0; l2 < k; l2++)
    {
        v.push_back(std::vector<PKT>());
        float yk = degToRad(-90.0f) + stepy*l2;
        for(int l1 = 0; l1 < n; l1++)
        {
            v[l2].push_back(setSphericalRotation(pos, r, step*l1, yk));
        }
    }
    draw(v, type);
}

void drawQuasiLine(Point A, Point B, int k, int n, float r, DRAW_TYPE type)
{
    if(A == B)
    {
        drawSphere(A, k, n*2, r, type);
        return;
    }

    std::vector<std::vector <PKT> > v;
    float step = M_PI / (float)(n-1);
    float stepy = M_PI / (float)(k-1);

    float d = dist(A,B);
    PKT pos = Punkt(0,0,-d);
    for(int l2 = 0; l2 < k; l2++)
    {
        v.push_back(std::vector<PKT>());
        float yk = degToRad(-90.0f) - stepy*l2;
        for(int l1 = 0; l1 < n; l1++)
        {
            v[l2].push_back(setSphericalRotation(PKT0, r, step*l1, yk));
        }
        v[l2].push_back(v[l2][v[l2].size()-1]+pos);
        for(int l1 = 0; l1 < n; l1++)
        {
            v[l2].push_back(setSphericalRotation(pos, r, M_PI+step*l1, yk));
        }
        v[l2].push_back(v[l2][0]+pos);
    }

    float k1 = radToDeg(getSphericalK1(A, B))+270;
    float k2 = radToDeg(getSphericalK2(A, B));
    glPushMatrix();
    glTranslatef(A.x, A.y, A.z);
    glRotatef(k1, 0.0f, 1.0f, 0.0f);
    glRotatef(k2, 1.0f, 0.0f, 0.0f);
    draw(v, type);
    glPopMatrix();
}

std::vector<fPKT> calculateTriangleRow2D(fPKT A, fPKT B, fPKT C, float r, int k)
{
    std::vector<fPKT> p;

    fPKT vec1, vec2;
    vec1 = rotate(A, C,-M_PI/2.0f)-A;
    vec2 = rotate(B, C, M_PI/2.0f)-B;
    vec1 = setLengthToVector(vec1, r);
    vec2 = setLengthToVector(vec2, r);

    float kat_max;
    fPKT E[3], F[3], O[3];
    E[0] = A+fPunkt(0.0f, r);
    F[0] = A+vec1;
    E[1] = C+vec1;
    F[1] = C+vec2;
    E[2] = B+vec2;
    F[2] = B+fPunkt(0.0f, r);
    O[0] = A;
    O[1] = C;
    O[2] = B;

    for(int l2 = 0; l2 < 3; l2++)
    {
        kat_max = getAngle(O[l2], E[l2], F[l2]);
        for(int l1 = 0; l1 <= k; l1++)
        {
            float percent = (float)l1 / (float)k;
            float kat = percent * kat_max;
            p.push_back(rotate(O[l2], E[l2], kat));
        }
    }

    return p;
}

void drawQuasiTriangle(Point A, Point B, Point C, int n, int k, float r, DRAW_TYPE type)
{
    if(arePointsNear(A,B))
    {
        drawQuasiLine(A, C, k, n, r, type);
        return;
    }

    std::vector<std::vector <PKT> > list;
    if(pointOnLine(A, B, C) == false)
    {
        PKT b3 = setLengthToVector(orthoToVec(B-A, C-A), 1.0f);
        PKT b1 = setLengthToVector(B-A, 1.0f);
        PKT b2 = setLengthToVector(orthoToVec(b1, b3), 1.0f);

        Projector pro(A, b1, b2, b3);
        fPKT fA = pro.transformWithoutZ(A);
        fPKT fB = pro.transformWithoutZ(B);
        fPKT fC = pro.transformWithoutZ(C);

        for(int l1 = 0; l1 < k; l1++)
        {
            float h, rr;
            if(k > 1)
            {
                h = -r + (2.0f*r/(float)(k-1))*(float)l1; // -r to r
                rr = cos((h/r)*(M_PI/2.0f))*r;
            }
            else
            {
                h = 0;
                rr = r;
            }
            std::vector <fPKT> v = calculateTriangleRow2D(fA, fB, fC, rr, n);
            list.push_back(std::vector<PKT>());
            for(unsigned int l2 = 0; l2 < v.size(); l2++)
            {
                list[l1].push_back(pro.detransformWithoutZ(v[l2])+b3*h);
            }
        }

        draw(list, type);

        if(type == DRAW_TYPE_FILLED)
        {
            glBegin(GL_TRIANGLES);
            renderVertex(A+b3*r);
            renderVertex(C+b3*r);
            renderVertex(B+b3*r);
            renderVertex(A-b3*r);
            renderVertex(B-b3*r);
            renderVertex(C-b3*r);
            glEnd();
        }
    }
    else // C is collinear
    {
        float AB = dists(A,B);
        float AC = dists(A,C);
        float BC = dists(B,C);
        if(AB >= AC)
        {
            if(AB >= BC) drawQuasiLine(A, B, k, n, r, type);
            else drawQuasiLine(B, C, k, n, r, type);
        }
        else
        {
            if(AC >= BC) drawQuasiLine(A, C, k, n, r, type);
            else drawQuasiLine(B, C, k, n, r, type);
        }
    }
}

}

