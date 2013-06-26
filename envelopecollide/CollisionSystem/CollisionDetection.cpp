/*
 * File:   CollisionDetection.cpp
 * Author: Przemysław Bągard
 *
 * Created on 18 marzec 2012, 15:37
 */
#include "CollisionDetection.h"
#include "Geometry/math.h"
#include <assert.h>

namespace EnvelopeCollide { namespace Functions {

/*********** Internal declarations **********/
// Internal declarations (in .cpp file) of collision detection functions. Those functions are inlined.
// External (with declarations in .h file) collision detection functions call those functions - those functions aren't inlined
/// Collision detection for displacemented point and point envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangePointPointI(const PKT& A, const PKT& v, const PKT& D, float r);
/// Collision detection for displacemented point and line segment envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangePointSegmentI(const PKT& s1, const PKT& v, const Segment& s2, float r);
/// Collision detection for displacemented point and triangle envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangePointTriangleI(const PKT& s1, const PKT& v, const Triangle& s2, float r);
/// Collision detection for displacemented line segment and point envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeSegmentPointI(const Segment& s1, const PKT& v, const PKT& s2, float r);
/// Collision detection for displacemented line segment and segment line envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeSegmentSegmentI(const Segment& s1, const PKT& v, const Segment& s2, float r);
/// Collision detection for displacemented line segment and triangle envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeSegmentTriangleI(const Segment& s1, const PKT& v, const Triangle& s2, float r);
/// Collision detection for displacemented triangle and point envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeTrianglePointI(const Triangle& s1, const PKT& v, const PKT& s2, float r);
/// Collision detection for displacemented triangle and line segment envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeTriangleSegmentI(const Triangle& s1, const PKT& v, const Segment& s2, float r);
/// Collision detection for displacemented triangle and triangle envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeTriangleTriangleI(const Triangle& s1, const PKT& v, const Triangle& s2, float r);

/********* Auxiliary structs ********/

/// Struct stores quadratic equation results.
struct ResultQuadraticEquation {
    unsigned char count;
    float value[2];
};

/********* Auxiliary functions ********/

/// Solves quadratic equation with one variable (a*x*x + b*x + c = 0).
/// If equation has one result, function returns this result twice.
inline ResultQuadraticEquation quadraticEquation(const double& a, const double& b, const double& c) {
    ResultQuadraticEquation result;
    double d = b*b-4*a*c;

    if(d >= 0.0) {
        result.count = 2;
        // r = (-b-sqrt(d))/2a   or     r = (-b+sqrt(d))/2a
        d = sqrt(d);
        result.value[0] = (-b-d)/(a*2);
        result.value[1] = (-b+d)/(a*2);
    } else {
        result.count = 0;
        if(a != 0.0f)
        {
            result.value[0] = -b/(2*a);
            result.value[1] = result.value[0];
        }
    }
    return result;
}

/// Intersection of line and circle
inline ResultQuadraticEquation lineAndCircleCollide(const fPKT& A, const fPKT& vec, const fPKT& O, const float& r) {
    // (A - O + vec*t)^2    ->   (P+vec*t)^2 = r^2
    fPKT P = A-O;
    return quadraticEquation(
            scalarProduct(vec, vec),
            2*scalarProduct(P, vec),
            scalarProduct(P, P)-r*r);
}

/// Intersection of line and sphere
inline ResultQuadraticEquation lineAndSphereCollide(const PKT& A, const PKT& vec, const PKT& O, const float& r) {
    // (A - O + vec*t)^2    ->   (P+vec*t)^2 = r^2
    PKT P = A-O;
    return quadraticEquation(
            scalarProduct(vec, vec),
            2*scalarProduct(P, vec),
            scalarProduct(P, P)-r*r);
}

/// Reverse results (for example we return result for collision point with line segment based on collision line segment with point function)
inline PierceRangeResult& swapResult(PierceRangeResult& result)
{
    if(result.onCollisionTrajectory)
    {
        result.vec = -result.vec;
        PKT temp = result.vMinPoint[0];
        result.vMinPoint[0] = result.vMinPoint[1];
        result.vMinPoint[1] = temp;
        temp = result.vMaxPoint[0];
        result.vMaxPoint[0] = result.vMaxPoint[1];
        result.vMaxPoint[1] = temp;
    }
    return result;
}

/// Degeneration: triangle -> line segment
inline Segment triangleDegenerate(const Triangle& tr)
{
    float d = dist(tr.a, tr.b);
    if(isZero(d)) return Segment(tr.a, tr.c);
    float t = dist(tr.a, tr.c)/d;
    if(isNegative(t)) return Segment(tr.b, tr.c);
    else if(t > 1.0f) return Segment(tr.a, tr.c);
    else return Segment(tr.a, tr.b);
}

/// Collision of 'p' point with triangle on the plane
inline PierceRangeResult pierceRangeTriangleSingleWithPoint(const PKT& p, const Projector& proj, const PKT& vecP)
{
    PierceRangeResult result;
    PKT A = proj.transform(p);
    if(A.x >= 0.0f && A.y >= 0.0f && A.x+A.y <= 1.0f)
    {
        result.onCollisionTrajectory = true;
        result.vMinDistance = A.z;
        result.vMaxDistance = A.z;
        result.vMinPoint[1] = p;
        result.vMaxPoint[1] = p;
        A.z = 0.0f;
        A = proj.detransform(A);
        A -= vecP;
        result.vMinPoint[0] = A;
        result.vMaxPoint[0] = A;
    }
    return result;
}

/*********** Collision detection functions implementation **********/

inline PierceRangeResult pierceRangePointPointI(const PKT& A, const PKT& v, const PKT& D, float r)
{
    assert(v != PKT0);
//    qDebug() << "\tEnter PointPoint";
    PierceRangeResult result;   // 'result.onCollisionTrajectory = false' na starcie
    ResultQuadraticEquation lineAndSphereResult = lineAndSphereCollide(A, v, D, r);

    if (lineAndSphereResult.count != 0) {
        result.onCollisionTrajectory = true;
        setMinMax<float>(lineAndSphereResult.value[0], lineAndSphereResult.value[1]);
        result.vMinDistance = lineAndSphereResult.value[0];
        result.vMinPoint[0] = A;
        result.vMinPoint[1] = D;
        result.vMaxDistance = lineAndSphereResult.value[1];
        result.vMaxPoint[0] = A;
        result.vMaxPoint[1] = D;
        result.vec = v;
    }

    return result;
}

inline PierceRangeResult pierceRangePointSegmentI(const PKT& s1, const PKT& v, const Segment& s2, float r) {
    assert(v != PKT0);
    PierceRangeResult result = pierceRangeSegmentPointI(s2, -v, s1, r);
    swapResult(result);
    return result;
}

inline PierceRangeResult pierceRangePointTriangleI(const PKT& s1, const PKT& v, const Triangle& s2, float r) {
    assert(v != PKT0);
    PierceRangeResult result = pierceRangeTrianglePointI(s2, -v, s1, r);
    swapResult(result);
    return result;
}

inline PierceRangeResult pierceRangeSegmentPointI(const Segment& s1, const PKT& v, const PKT& s2, float r) {
    assert(v != PKT0);
//    qDebug() << "\tEnter SegmentPoint";

    PKT b1 = setLengthToVector(v);
    PKT b2 = s1.b - s1.a;

    PierceRangeResult result;
    PKT ortho = setLengthToVector(orthoToVec(b1, b2));
    if(isZero(ortho.x) && isZero(ortho.y) && isZero(ortho.z))
    {
        result = pierceRangePointPointI(s1.a, v, s2, r);
        if(!arePointsNear(s1.a, s1.b))
            pierceRangeMerge(result, pierceRangePointPointI(s1.b, v, s2, r));
        return result;
    }
    b2 = setLengthToVector(orthoToVec(ortho, b1));

    Projector proj(s1.a, b1, b2, ortho);
    PKT projA = proj.transform(s2);
    // Creates section through plane
    if(abs(projA.z) > r)
        return result;

    // Creates a 2D situation
    // Calculates radius of cirle with center '(projaA.x, projaB.y, 0)'
    float r2 = sqrt(r*r - projA.z*projA.z);

//    fPKT projD = fPKT0;
    fPKT A = pointWithoutZ(projA);
    fPKT u2 = proj.transformWithoutZ(s1.b);      // 's1.a=(0,0)'
    fPKT v2 = proj.transformWithoutZ(s1.a+v);    // 's1.a=(0,0)'

    // solves equation - returns moments, in which line parallel to vector 'u' intersects sphere in exactly one point
    float f = scalarProduct(u2,u2);
    float g = -2*scalarProduct(A, u2);
    float h = 2*scalarProduct(v2,u2);
    float i = scalarProduct(A,A) -r2*r2;
    float j = -2*scalarProduct(A,v2);
    float k = scalarProduct(v2,v2);
    ResultQuadraticEquation equation2 = quadraticEquation(
            h*h - 4*f*k,
            2*g*h -4*f*j,
            g*g - 4*f*i);

    // 'equation2' has at least one result - otherwise procedure ended before creation of 2D situation.

    ResultQuadraticEquation equation;
    result.vMinPoint[1] = s2;
    result.vMaxPoint[1] = s2;
    setMinMax<float>(equation2.value[0], equation2.value[1]);

    // -b/2*a
    float _2a = 2*f;

    // result for tmin
    float b = g + h*equation2.value[0];
    float t2 = -b/_2a;
    if(t2 < 0.0f)   // below; we replace current value 't2' to 0
    {
        equation = lineAndCircleCollide(fPKT0, v2, A, r2);
        if(equation.count != 0)
        {
            result.onCollisionTrajectory = true;
            result.vMinDistance = min(equation.value[0], equation.value[1]);
        }
        t2 = 0.0f;
    }
    else if(t2 > 1.0f)   // above; we replace current value 't2' to 1
    {
        equation = lineAndCircleCollide(u2, v2, A, r2);
        if(equation.count != 0)
        {
            result.onCollisionTrajectory = true;
            result.vMinDistance = min(equation.value[0], equation.value[1]);
        }
        t2 = 1.0f;
    }
    else
    {
        result.onCollisionTrajectory = true;
        result.vMinDistance = equation2.value[0];
    }
    result.vMinPoint[0] = s1.a + (s1.b-s1.a)*t2;

    // result for tmax
    b = g + h*equation2.value[1]; // greater equation result
    t2 = -b/_2a;
    if(t2 < 0.0f)   // below; we replace current value 't2' to 0
    {
        equation = lineAndCircleCollide(fPKT0, v2, A, r2);
        if(equation.count != 0)
        {
            result.onCollisionTrajectory = true;
            result.vMaxDistance = max(equation.value[0], equation.value[1]);
        }
        t2 = 0.0f;
    }
    else if(t2 > 1.0f)   // above; we replace current value 't2' to 1
    {
        equation = lineAndCircleCollide(u2, v2, A, r2);
        if(equation.count != 0)
        {
            result.onCollisionTrajectory = true;
            result.vMaxDistance = max(equation.value[0], equation.value[1]);
        }
        t2 = 1.0f;
    }
    else
    {
        result.onCollisionTrajectory = true;
        result.vMaxDistance = equation2.value[1];
    }
    result.vMaxPoint[0] = s1.a + (s1.b-s1.a)*t2;
    /////////////////////////////////////
    result.vec = v;
    return result;
}

/// Collision detection for displacemented line segment and cylinder (v != PKT0, r != 0)
inline PierceRangeResult pierceRangeSegmentCylinderI(const Segment& s1, const PKT& v, const Segment& s2, float r)
{
    assert(v != PKT0);
//    qDebug() << "\tEnter SegmentCylinder";
    PierceRangeResult result;
    // 'min' and 'max' are initialized with 'INFINITY' and '-INFINITY' values.

    PKT vc = s2.b-s2.a;
    PKT vb = orthoToVec(vc, v);

    if(!(isZero(vb.x) && isZero(vb.y) && isZero(vb.z)))
    {
        // 'A' has zero depth ('A.z = 0'), and 'B.z = 1.0f'
        PKT va = orthoToVec(vb, vc);
        va = setLengthToVector(va);
        vb = setLengthToVector(vb);

        Projector proj(s2.a, va, vb, vc);

        assert(proj.isValid());

        // At this point we know, that there was no collision at the start.
        // We have 2D situation. In this situation cylinder projects to circle.

        PKT D2 = proj.transform(s1.a);
        PKT E2 = proj.transform(s1.b);
        PKT v2 = proj.transform(s1.a+v)-D2;
        PKT u2 = E2-D2;
        fPKT D3 = fPunkt(D2.x, D2.y); fPKT u3 = fPunkt(u2.x, u2.y); fPKT v3 = fPunkt(v2.x, v2.y);

        if(!parallelVector(u3, v3))     // if 'u3 x v3' not determine a plane we can finish now
        {
            // Find the optimal solution to the 2D problem (skip third dimmension).
            // A2+v2*t1 + u2*t2 = P     =>      (O-P)scalar = r*r       =>      delta = 0
            // equation I
            double D3D3 = scalarProductDouble(D3, D3); double D3u3 = scalarProductDouble(D3, u3); double D3v3 = scalarProductDouble(D3, v3);
            double v3u3 = scalarProductDouble(v3, u3); double v3v3 = scalarProductDouble(v3, v3); double u3u3 = scalarProductDouble(u3, u3);
            // equation II
            ResultQuadraticEquation equation = quadraticEquation(
                    v3u3*v3u3 - u3u3*v3v3,
                    (D3u3*v3u3 - u3u3*D3v3)*2.0f,
                    D3u3*D3u3 - u3u3*(D3D3 - r*r));

            if(equation.count != 0)
            {
                setMinMax<float>(equation.value[0], equation.value[1]);
                //float g = min(equation.value[0], equation.value[1]);

                 // go back to equation I with t1 = g     => we calculate h = -b/2a
                float t2 = (-2*(D3u3 + v3u3*equation.value[0]))/(2*u3u3);
             //   result.vMinDistance = t2;
                PKT P = Punkt(0.0f, 0.0f, D2.z + v2.z*equation.value[0] + u2.z*t2);
                if(t2 >= 0.0f && t2 <= 1.0f && P.z >= 0.0f && P.z <= 1.0f)
                {
                    result.onCollisionTrajectory = true;
                    result.vMinDistance = equation.value[0];
                    result.vMinPoint[0] = s1.a + (s1.b-s1.a)*t2;
                    result.vMinPoint[1] = proj.detransform(P);
                }
        //        else
        //            result.vMinDistance = INFINITY;

                //g = max(equation.value[0], equation.value[1]);
                t2 = (-2*(D3u3 + v3u3*equation.value[1]))/(2*u3u3);
                P = Punkt(0.0f, 0.0f, D2.z + v2.z*equation.value[1] + u2.z*t2);
                if(t2 >= 0.0f && t2 <= 1.0f && P.z >= 0.0f && P.z <= 1.0f)
                {
                    result.onCollisionTrajectory = true;
                    result.vMaxDistance = equation.value[1];
                    result.vMaxPoint[0] = s1.a + (s1.b-s1.a)*t2;
                    result.vMaxPoint[1] = proj.detransform(P);
                }
         //       else
         //           result.vMaxDistance = -INFINITY;
            }
        }

        float tt1[2], tt2[2];
        PKT pp0[2], pp1[2], pp2[2];
        ResultQuadraticEquation equation;

   //     if(t2 < 0.0f)
        {
            equation = lineAndCircleCollide(D3, v3, fPKT0, r);
            if(equation.count != 0)
            {
                if(equation.value[0] < equation.value[1]) { tt1[0] = equation.value[0]; tt2[0] = equation.value[1]; }
                else { tt1[0] = equation.value[1]; tt2[0] = equation.value[0]; }
                //tz = D2.z + v2.z * tt1[0];
                pp0[0] = s1.a;
                PKT P = Punkt(0.0f, 0.0f, D2.z + v2.z * tt1[0]);
                //criteria[0] = equation.count > 0 && P.z >= 0.0f && P.z <= 1.0f;
                if(P.z >= 0.0f && P.z <= 1.0f)
                {
                    result.onCollisionTrajectory = true;
                    pp1[0] = proj.detransform(P);
                }
                else tt1[0] = INFINITY;

                P = Punkt(0.0f, 0.0f, D2.z + v2.z * tt2[0]);
                if(P.z >= 0.0f && P.z <= 1.0f)
                {
                    result.onCollisionTrajectory = true;
                    pp2[0] = proj.detransform(P);
                }
                else
                    tt2[0] = -INFINITY;
            }
            else
            {
                tt1[0] = INFINITY;
                tt2[0] = -INFINITY;
            }
        }
   //     if(t2 > 1.0f)
        {
            equation = lineAndCircleCollide(D3+u3, v3, fPKT0, r);
            if(equation.count != 0)
            {
                if(equation.value[0] < equation.value[1]) { tt1[1] = equation.value[0]; tt2[1] = equation.value[1]; }
                else { tt1[1] = equation.value[1]; tt2[1] = equation.value[0]; }
              //  tz = E2.z + v2.z * tt1[1];
                pp0[1] = s1.b;
                PKT P = Punkt(0.0f, 0.0f, E2.z + v2.z * tt1[1]);
                if(P.z >= 0.0f && P.z <= 1.0f)
                {
                    result.onCollisionTrajectory = true;
                    pp1[1] = proj.detransform(P);
                }
                else
                    tt1[1] = INFINITY;

                P = Punkt(0.0f, 0.0f, E2.z + v2.z * tt2[1]);
                if(P.z >= 0.0f && P.z <= 1.0f)
                {
                    result.onCollisionTrajectory = true;
                    pp2[1] = proj.detransform(P);
                }
                else
                    tt2[1] = -INFINITY;
             //   criteria[1] = equation.count > 0 && tz >= 0.0f && tz <= 1.0f;
            }
            else
            {
                tt1[1] = INFINITY;
                tt2[1] = -INFINITY;
            }
        }

        for(int l1 = 0; l1 < 2; l1++)
        {
            // istotne założenie - tt oraz result m
            if(tt1[l1] < result.vMinDistance)
            {
                result.vMinDistance = tt1[l1];
                result.vMinPoint[0] = pp0[l1];
                result.vMinPoint[1] = pp1[l1];
            }
            if(tt2[l1] > result.vMaxDistance)
            {
                result.vMaxDistance = tt2[l1];
                result.vMaxPoint[0] = pp0[l1];
                result.vMaxPoint[1] = pp2[l1];
            }
        }
    }
    result.vec = v;
    return result;
}

inline PierceRangeResult pierceRangeSegmentSegmentI(const Segment& s1, const PKT& v, const Segment& s2, float r)
{
    assert(v != PKT0);

    PierceRangeResult result = pierceRangeSegmentCylinderI(s1, v, s2, r);
    pierceRangeMerge(result, pierceRangeSegmentPointI(s1, v, s2.a, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(s1, v, s2.b, r));

    return result;
}

inline PierceRangeResult pierceRangeSegmentTriangleI(const Segment& s1, const PKT& v, const Triangle& s2, float r)
{
    assert(v != PKT0);

    PierceRangeResult result = pierceRangeTriangleSegmentI(s2, -v, s1, r);
    swapResult(result);
    return result;
}

inline PierceRangeResult pierceRangeTrianglePointI(const Triangle& s1, const PKT& v, const PKT& s2, float r)
{
    assert(v != PKT0);

    PierceRangeResult result;
    PKT vecP = orthoToVec(s1.b-s1.a, s1.c-s1.a);
    if(isZero(vecP.x) && isZero(vecP.y) && isZero(vecP.z))
    {
        // degenerated triangle
        if(linesParallel(s1.a, s1.b, s1.a, s1.c))
            return pierceRangeSegmentPointI(triangleDegenerate(s1), v, s2, r);
    }

    // the bases of the cuboid
    vecP = setLengthToVector(vecP, r);
    Projector proj(s1.a+vecP, s1.b-s1.a, s1.c-s1.a, v);
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2, proj, vecP));
    vecP = -vecP;
    proj.setCenter(s1.a+vecP);
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2, proj, vecP));

    // edges
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.a, s1.b), v, s2, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.a, s1.c), v, s2, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.b, s1.c), v, s2, r));
    // 'pierceRangeSegmentPointI' calls also 'pierceRangePointPointI'.
    result.vec = v;
    return result;
}

inline PierceRangeResult pierceRangeTriangleSegmentI(const Triangle& s1, const PKT& v, const Segment& s2, float r)
{
    assert(v != PKT0);

    PierceRangeResult result;
    // the bases of the cuboid
    PKT vecP = orthoToVec(s1.b-s1.a, s1.c-s1.a);
    if(isZero(vecP.x) && isZero(vecP.y) && isZero(vecP.z))
    {
        // degenerated figures
        if(arePointsNear(s2.a, s2.b)) return pierceRangeTrianglePointI(s1, v, s2.a, r);
        if(linesParallel(s1.a, s1.b, s1.a, s1.c))
            return pierceRangeSegmentSegmentI(triangleDegenerate(s1), v, s2, r);
    }

    // the bases of the cuboid
    vecP = setLengthToVector(vecP, r);
    Projector proj(s1.a+vecP, s1.b-s1.a, s1.c-s1.a, v);
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.a, proj, vecP));
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.b, proj, vecP));
    vecP = -vecP;
    proj.setCenter(s1.a+vecP);
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.a, proj, vecP));
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.b, proj, vecP));

    // edges, vertexes
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(Segment(s1.a, s1.c), v, s2, r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(Segment(s1.a, s1.b), v, s2, r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(Segment(s1.b, s1.c), v, s2, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.a, s1.c), v, s2.a, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.a, s1.c), v, s2.b, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.a, s1.b), v, s2.a, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.a, s1.b), v, s2.b, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.b, s1.c), v, s2.a, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(Segment(s1.b, s1.c), v, s2.b, r));

    result.vec = v;
    return result;
}

inline PierceRangeResult pierceRangeTriangleTriangleSimpleI(const Triangle& s1, const PKT& v, const Triangle& s2, float r)
{
    PierceRangeResult result;
    // the bases of the cuboid
    {
        PKT vecP = orthoToVec(s1.b-s1.a, s1.c-s1.a);
        if(isZero(vecP.x) && isZero(vecP.y) && isZero(vecP.z))
        {
            // degenerated figures
            if(linesParallel(s1.a, s1.b, s1.a, s1.c))
                return pierceRangeSegmentTriangleI(triangleDegenerate(s1), v, s2, r);
            if(linesParallel(s2.a, s2.b, s2.a, s2.c))
                return pierceRangeTriangleSegmentI(s1, v, triangleDegenerate(s2), r);
        }
        vecP = setLengthToVector(vecP, r);

        // projects on first triangle plane
        {
            Projector proj(s1.a+vecP, s1.b-s1.a, s1.c-s1.a, v);
            pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.a, proj, vecP));
            pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.b, proj, vecP));
            pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.c, proj, vecP));
            vecP = -vecP;
            proj.setCenter(s1.a+vecP);
            pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.a, proj, vecP));
            pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.b, proj, vecP));
            pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.c, proj, vecP));
        }
    }
    return result;
}

inline PierceRangeResult pierceRangeTriangleTriangleI(const Triangle& s1, const PKT& v, const Triangle& s2, float r)
{
    assert(v != PKT0);

    PKT v2 = -v;

    PierceRangeResult result = pierceRangeTriangleTriangleSimpleI(s1, v, s2, r);
    PierceRangeResult reversed_result = pierceRangeTriangleTriangleSimpleI(s2,v2, s1, r);

    // checks collisions for edges and vertexes
    Segment seg[3][2] = {
        {Segment(s1.a, s1.b), Segment(s2.a, s2.b)},
        {Segment(s1.a, s1.c), Segment(s2.a, s2.c)},
        {Segment(s1.b, s1.c), Segment(s2.b, s2.c)}
    };

    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[0][0], v, seg[0][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[0][0], v, seg[1][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[0][0], v, seg[2][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[1][0], v, seg[0][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[1][0], v, seg[1][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[1][0], v, seg[2][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[2][0], v, seg[0][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[2][0], v, seg[1][1], r));
    pierceRangeMerge(result, pierceRangeSegmentCylinderI(seg[2][0], v, seg[2][1], r));

    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[0][0], v, s2.a, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[0][0], v, s2.b, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[0][0], v, s2.c, r));

    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[1][0], v, s2.a, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[1][0], v, s2.b, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[1][0], v, s2.c, r));

    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[2][0], v, s2.a, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[2][0], v, s2.b, r));
    pierceRangeMerge(result, pierceRangeSegmentPointI(seg[2][0], v, s2.c, r));

    // at the end we merge normal and reversed results
    pierceRangeMerge(result, swapResult(reversed_result));
    result.vec = v;
    return result;
}

inline PierceRangeResult pierceRangeTriangleTriangleIDeprecated(const Triangle& s1, const PKT& v, const Triangle& s2, float r)
{
    assert(v != PKT0);

    PierceRangeResult result;
    // the bases of the cuboid
    PKT vecP = orthoToVec(s1.b-s1.a, s1.c-s1.a);
    if(isZero(vecP.x) && isZero(vecP.y) && isZero(vecP.z))
    {
        // degenerated figures
        if(linesParallel(s1.a, s1.b, s1.a, s1.c))
            return pierceRangeSegmentTriangleI(triangleDegenerate(s1), v, s2, r);
        if(linesParallel(s2.a, s2.b, s2.a, s2.c))
            return pierceRangeTriangleSegmentI(s1, v, triangleDegenerate(s2), r);
    }

    // the bases of the cuboid
    vecP = setLengthToVector(vecP, r);
    Projector proj(s1.a+vecP, s1.b-s1.a, s1.c-s1.a, v);
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.a, proj, vecP));
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.b, proj, vecP));
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.c, proj, vecP));
    vecP = -vecP;
    proj.setCenter(s1.a+vecP);
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.a, proj, vecP));
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.b, proj, vecP));
    pierceRangeMerge(result, pierceRangeTriangleSingleWithPoint(s2.c, proj, vecP));

    // edges, vertexes
    pierceRangeMerge(result, pierceRangeSegmentTriangleI(Segment(s1.a, s1.c), v, s2, r));
    pierceRangeMerge(result, pierceRangeSegmentTriangleI(Segment(s1.a, s1.b), v, s2, r));
    pierceRangeMerge(result, pierceRangeSegmentTriangleI(Segment(s1.b, s1.c), v, s2, r));

    result.vec = v;
    return result;
}



/////////////////////////////////////////////
/// External collision detection functions

PierceRangeResult pierceRangePointPoint(const PKT& A, const PKT& v, const PKT& D, float r)
{
    return pierceRangePointPointI(A, v, D, r);
}

PierceRangeResult pierceRangePointSegment(const PKT& s1, const PKT& v, const Segment& s2, float r)
{
    return pierceRangePointSegmentI(s1, v, s2, r);
}

PierceRangeResult pierceRangePointTriangle(const PKT& s1, const PKT& v, const Triangle& s2, float r)
{
    return pierceRangePointTriangleI(s1, v, s2, r);
}

PierceRangeResult pierceRangeSegmentPoint(const Segment& s1, const PKT& v, const PKT& s2, float r)
{
    return pierceRangeSegmentPointI(s1, v, s2, r);
}

PierceRangeResult pierceRangeSegmentSegment(const Segment& s1, const PKT& v, const Segment& s2, float r)
{
    return pierceRangeSegmentSegmentI(s1, v, s2, r);
}

PierceRangeResult pierceRangeSegmentTriangle(const Segment& s1, const PKT& v, const Triangle& s2, float r)
{
    return pierceRangeSegmentTriangleI(s1, v, s2, r);
}

PierceRangeResult pierceRangeTrianglePoint(const Triangle& s1, const PKT& v, const PKT& s2, float r)
{
    return pierceRangeTrianglePointI(s1, v, s2, r);
}

PierceRangeResult pierceRangeTriangleSegment(const Triangle& s1, const PKT& v, const Segment& s2, float r)
{
    return pierceRangeTriangleSegmentI(s1, v, s2, r);
}

PierceRangeResult pierceRangeTriangleTriangle(const Triangle& s1, const PKT& v, const Triangle& s2, float r)
{
    return pierceRangeTriangleTriangleI(s1, v, s2, r);
}

PierceRangeResult pierceRangeTriangleTriangleDeprecated(const Triangle& s1, const PKT& v, const Triangle& s2, float r)
{
    return pierceRangeTriangleTriangleIDeprecated(s1, v, s2, r);
}


}}
