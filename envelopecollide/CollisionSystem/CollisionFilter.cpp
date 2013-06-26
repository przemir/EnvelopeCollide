/// Author: Przemysław Bągard

#include "CollisionFilter.h"

namespace EnvelopeCollide { namespace Functions {

Projector makeCubeProjectorFromMoveVector(PKT vec)
{
    vec = setLengthToVector(vec);
    PKT ortho = orthoToVec(vec, Punkt(0,1,0));
    if(isZero(ortho.x) && isZero(ortho.y) && isZero(ortho.z))
        ortho = orthoToVec(vec, Punkt(-1,0,0));
    PKT after = orthoToVec(ortho, vec);
    return Projector(PKT0, vec, after);
}

/// Function assumes that 'min' and 'max' was initialized with coordinates of any point which belongs to the set.
void getProjectorMinMax(const Projector& proj, PKT p, PKT& min, PKT& max)
{
    p = proj.transform(p);
    if(p.x < min.x) min.x = p.x;
    else if(p.x > max.x) max.x = p.x;
    if(p.y < min.y) min.y = p.y;
    else if(p.y > max.y) max.y = p.y;
    if(p.z < min.z) min.z = p.z;
    else if(p.z > max.z) max.z = p.z;
}

inline void getProjectorMinMaxRExecute(PKT& min, PKT& max, const float& r)
{
    min.x -= r;
    min.y -= r;
    min.z -= r;
    max.x += r;
    max.y += r;
    max.z += r;
}

bool checkMoveVectorProjectorFilterPoint(const Projector& proj, PKT min, PKT max, const float& r, const PKT& p1)
{
    getProjectorMinMaxRExecute(min, max, r);
    min = proj.detransform(min);
    max = proj.detransform(max);
    Plane pl;
    for(int l1 = 0; l1 < 3; l1++)
    {
        pl = Plane(proj.getBase(l1), calculatePlaneD(proj.getBase(l1), min));
        if(calculateSideOfPlane(pl, p1) < 0)
            return false;
        pl = Plane(proj.getBase(l1), calculatePlaneD(proj.getBase(l1), max));
        if(calculateSideOfPlane(pl, p1) > 0)
            return false;
    }
    return true;
}

bool checkMoveVectorProjectorFilterSegment(const Projector& proj, PKT min, PKT max, const float& r, const Segment& s)
{
    getProjectorMinMaxRExecute(min, max, r);
    min = proj.detransform(min);
    max = proj.detransform(max);
    Plane pl;
    for(int l1 = 0; l1 < 3; l1++)
    {
        pl = Plane(proj.getBase(l1), calculatePlaneD(proj.getBase(l1), min));
        if(calculateSideOfPlane(pl, s.a) < 0 && calculateSideOfPlane(pl, s.b) < 0)
            return false;
        pl = Plane(proj.getBase(l1), calculatePlaneD(proj.getBase(l1), max));
        if(calculateSideOfPlane(pl, s.a) > 0 && calculateSideOfPlane(pl, s.b) > 0)
            return false;
    }
    return true;
}

bool checkMoveVectorProjectorFilterTriangle(const Projector& proj, PKT min, PKT max, const float& r, const Triangle& s)
{
    getProjectorMinMaxRExecute(min, max, r);
    min = proj.detransform(min);
    max = proj.detransform(max);
    Plane pl;
    for(int l1 = 0; l1 < 3; l1++)
    {
        pl = Plane(proj.getBase(l1), calculatePlaneD(proj.getBase(l1), min));
        if(calculateSideOfPlane(pl, s.a) < 0 && calculateSideOfPlane(pl, s.b) < 0 && calculateSideOfPlane(pl, s.c) < 0)
            return false;
       // pl = Plane(proj.getBase(l1), calculatePlaneD(proj.getBase(l1), max));
        pl.D = calculatePlaneD(proj.getBase(l1), max);
        if(calculateSideOfPlane(pl, s.a) > 0 && calculateSideOfPlane(pl, s.b) > 0 && calculateSideOfPlane(pl, s.c) > 0)
            return false;
    }
    return true;
}

bool checkSimpleProjectorFilterPoint(PKT min, PKT max, const float& r, const PKT& s)
{
    getProjectorMinMaxRExecute(min, max, r);
    return !(s.x < min.x || s.x > max.x || s.y < min.y || s.y > max.y || s.z < min.z || s.z > max.z);
}

bool checkSimpleProjectorFilterSegment(PKT min, PKT max, const float& r, const Segment& s)
{
    getProjectorMinMaxRExecute(min, max, r);

    if(s.a.x < min.x && s.b.x < min.x)
        return false;

    if(s.a.x > max.x && s.b.x > max.x)
        return false;

    if(s.a.y < min.y && s.b.y < min.y)
        return false;

    if(s.a.y > max.y && s.b.y > max.y)
        return false;

    if(s.a.z < min.z && s.b.z < min.z)
        return false;

    if(s.a.z > max.z && s.b.z > max.z)
        return false;

    return true;
}

bool checkSimpleProjectorFilterTriangle(PKT min, PKT max, const float& r, const Triangle& s)
{
    getProjectorMinMaxRExecute(min, max, r);

    if(s.a.x < min.x && s.b.x < min.x && s.c.x < min.x)
        return false;

    if(s.a.x > max.x && s.b.x > max.x && s.c.x > max.x)
        return false;

    if(s.a.y < min.y && s.b.y < min.y && s.c.y < min.y)
        return false;

    if(s.a.y > max.y && s.b.y > max.y && s.c.y > max.y)
        return false;

    if(s.a.z < min.z && s.b.z < min.z && s.c.z < min.z)
        return false;

    if(s.a.z > max.z && s.b.z > max.z && s.c.z > max.z)
        return false;

    return true;
}

/// Generate simple bounding sphere for triangle envelope.
inline void createBoundingSphereForTriangle(Mesh& target, const Mesh& source)
{
    PKT min = source.p[0];
    PKT max = source.p[0];
    if(min.x > source.p[1].x) min.x = source.p[1].x;
    if(min.x > source.p[2].x) min.x = source.p[2].x;
    if(min.y > source.p[1].y) min.y = source.p[1].y;
    if(min.y > source.p[2].y) min.y = source.p[2].y;
    if(min.z > source.p[1].z) min.z = source.p[1].z;
    if(min.z > source.p[2].z) min.z = source.p[2].z;
    if(max.x < source.p[1].x) max.x = source.p[1].x;
    if(max.x < source.p[2].x) max.x = source.p[2].x;
    if(max.y < source.p[1].y) max.y = source.p[1].y;
    if(max.y < source.p[2].y) max.y = source.p[2].y;
    if(max.z < source.p[1].z) max.z = source.p[1].z;
    if(max.z < source.p[2].z) max.z = source.p[2].z;
    target.p[0] = (min+max)/2.0f;
    target.r = dist(target.p[0], min) + source.r;
}

void createBoundingSphereSimple(Mesh& target, const Mesh& source)
{
    target.type = EnvelopeCollide::POINT;
    switch(source.type)
    {
    case EnvelopeCollide::POINT:
        target.p[0] = source.p[0];
        target.r = source.r;
        break;
    case EnvelopeCollide::LINE:
        target.p[0] = (source.p[0]+source.p[1])/2.0f;
        target.r = dist(target.p[0], source.p[0]) + source.r;
        break;
    case EnvelopeCollide::TRIANGLE:
    {
        createBoundingSphereForTriangle(target, source);
        break;
    }
    default:
        // unknown type
        assert(false);
        break;
    }
}

void createBoundingSphere(Mesh& target, const Mesh& source)
{
    target.type = EnvelopeCollide::POINT;
    switch(source.type)
    {
    case EnvelopeCollide::POINT:
        target.p[0] = source.p[0];
        target.r = source.r;
        break;
    case EnvelopeCollide::LINE:
        target.p[0] = (source.p[0]+source.p[1])/2.0f;
        target.r = dist(target.p[0], source.p[0]) + source.r;
        break;
    case EnvelopeCollide::TRIANGLE:
    {
        for (int i = 0; i < 3; ++i) {
            float W = scalarProduct(source.p[(i+1)%3]-source.p[i], source.p[(i+2)%3]-source.p[i]);
            if(W <= 0.0f)
            {
                target.p[0] = (source.p[(i+1)%3]+source.p[(i+2)%3])/2.0f;
                target.r = dist(target.p[0], source.p[(i+1)%3]) + source.r;
                return;
            }
        }

        float od = dist(source.p[1],source.p[0]);
        PKT n = setLengthToVector(orthoToVec(source.p[1]-source.p[0], source.p[2]-source.p[0]), od);
        PKT u = setLengthToVector(orthoToVec(source.p[1]-source.p[0], n), od);
        Projector proj(source.p[0], source.p[1]-source.p[0], u, n);

        if(!proj.isValid())
        {
            target.type = EnvelopeCollide::POINT;
            createBoundingSphereForTriangle(target, source);
            return;
        }

        fPKT c2 = proj.transformWithoutZ(source.p[2]);
        fPKT u2 = orthogonalVector(c2);

        float t2 = (0.5f-c2.x/2.0f)/u2.x;
        fPKT o2 = fPKT(0.5f, (c2.y/2.0f) + t2*u2.y);

//        assert(inTriangle(p[0], p[1], c2, o2)); // crashes because of measurement errors - epsilon is needed
        target.p[0] = proj.detransformWithoutZ(o2);
        target.r = dist(target.p[0], source.p[0]) + source.r + 0.0001;
        break;
    }
    default:
        // unknown type
        assert(false);
        break;
    }
}




}}
