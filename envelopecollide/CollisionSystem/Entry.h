/// Author: Przemysław Bągard

#ifndef ENTRY_H
#define ENTRY_H

#include "Const.h"
#include "Mesh.h"
#include "PierceRangeResult.h"
#include "CollisionDetection.h"
#include "CollisionFilter.h"

namespace EnvelopeCollide
{

/// Object with respect to which we checks if objects from structure collide with this object.
struct Entry
{
    Entry() { back_max = 0.0f; front_max = 1.0f; }

    /// Solid
    EnvelopeCollide::Mesh mesh;
    /// Movement vector
    PKT vec;
    /// earliest moment of collision
    float back_max;
    /// latest moment of collision
    float front_max;

    /// Checks if collision occurs and returns result.
    Functions::PierceRangeResult collision(const EnvelopeCollide::Mesh& atom);

    /// Sets 'this' from 'atom' object.
    void makeFromAtom(const EnvelopeCollide::Mesh& atom);
};

}

/// implementation

namespace EnvelopeCollide
{


inline Functions::PierceRangeResult Entry::collision(const EnvelopeCollide::Mesh &atom)
{
    float r = mesh.r + atom.r + EnvelopeCollide::COLLISION_MIN_R;
    switch(mesh.type)
    {
    case EnvelopeCollide::POINT:
    {
        switch(atom.type)
        {
        case EnvelopeCollide::POINT: return Functions::pierceRangePointPoint(mesh.p[0], vec, atom.p[0], r); break;
        case EnvelopeCollide::LINE: return Functions::pierceRangePointSegment(mesh.p[0], vec, Segment(atom.p[0],atom.p[1]), r); break;
        case EnvelopeCollide::TRIANGLE: return Functions::pierceRangePointTriangle(mesh.p[0], vec, Triangle(atom.p[0], atom.p[1], atom.p[2]), r); break;
        default: break;
        }
        break;
    }
    case EnvelopeCollide::LINE:
    {
        switch(atom.type)
        {
        case EnvelopeCollide::POINT: return Functions::pierceRangeSegmentPoint(Segment(mesh.p[0],mesh.p[1]), vec, atom.p[0], r); break;
        case EnvelopeCollide::LINE: return Functions::pierceRangeSegmentSegment(Segment(mesh.p[0],mesh.p[1]), vec, Segment(atom.p[0],atom.p[1]), r); break;
        case EnvelopeCollide::TRIANGLE: return Functions::pierceRangeSegmentTriangle(Segment(mesh.p[0],mesh.p[1]), vec, Triangle(atom.p[0], atom.p[1], atom.p[2]), r); break;
        default: break;
        }
        break;
    }
    case EnvelopeCollide::TRIANGLE:
    {
        switch(atom.type)
        {
        case EnvelopeCollide::POINT: return Functions::pierceRangeTrianglePoint(Triangle(mesh.p[0],mesh.p[1],mesh.p[2]), vec, atom.p[0], r); break;
        case EnvelopeCollide::LINE: return Functions::pierceRangeTriangleSegment(Triangle(mesh.p[0],mesh.p[1],mesh.p[2]), vec, Segment(atom.p[0],atom.p[1]), r); break;
        case EnvelopeCollide::TRIANGLE: return Functions::pierceRangeTriangleTriangle(Triangle(mesh.p[0],mesh.p[1],mesh.p[2]), vec, Triangle(atom.p[0], atom.p[1], atom.p[2]), r); break;
        default: break;
        }
        break;
    }
    default:
        break;
    };
    return Functions::PierceRangeResult();
}

inline void Entry::makeFromAtom(const EnvelopeCollide::Mesh &atom)
{
    for(int i = 0; i < atom.pCount(); ++i)
        mesh.p[i] = atom.p[i];
    mesh.type = atom.type;
    mesh.r = atom.r;
}


}



#endif // ENTRY_H
