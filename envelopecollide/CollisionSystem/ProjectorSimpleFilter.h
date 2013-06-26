/// Author: Przemysław Bągard

#ifndef PROJECTORSIMPLEFILTER_H
#define PROJECTORSIMPLEFILTER_H

#include "FilterFunction.h"
#include "Entry.h"
#include "Mesh.h"
#include "CollisionFilter.h"
#include "Geometry/Projector.h"

namespace EnvelopeCollide { namespace System {

/// Pre-filter class using tafle intersection method method with planes parallel to global coordinate system (definition of taflas in "CollisionFilter.h").
class ProjectorSimpleFilter : public FilterFunction
{
public:
    ProjectorSimpleFilter();

    inline virtual void init(const Entry& entry);
    inline virtual bool isOk(const Entry& entry, const Mesh& mesh);

    /// Updates the current values 'min' and 'max' by comparing those points with point 'p'
    void getMinMax(const PKT &p);

    /// Returns left, down, bottom cuboid coordinates
    PKT min;

    /// Returns right, up, top cuboid coordinates
    PKT max;
};

}}


namespace EnvelopeCollide { namespace System {

inline ProjectorSimpleFilter::ProjectorSimpleFilter() : min(Punkt(INFINITY, INFINITY, INFINITY)), max(Punkt(-INFINITY, -INFINITY, -INFINITY))
{
}

inline void ProjectorSimpleFilter::init(const EnvelopeCollide::Entry &entry)
{
    for(int l1 = 0; l1 <= entry.mesh.pCount(); l1++)
    {
        getMinMax(entry.mesh.p[l1]+entry.vec*entry.back_max);
        getMinMax(entry.mesh.p[l1]+entry.vec*entry.front_max);
    }
}

inline bool ProjectorSimpleFilter::isOk(const EnvelopeCollide::Entry &entry, const EnvelopeCollide::Mesh &mesh)
{
    bool filter = false;
    float r = entry.mesh.r + mesh.r + EnvelopeCollide::COLLISION_MIN_R;
    switch(mesh.type)
    {
    case EnvelopeCollide::POINT: filter = Functions::checkSimpleProjectorFilterPoint(min, max, r, mesh.p[0]); break;
    case EnvelopeCollide::LINE: filter = Functions::checkSimpleProjectorFilterSegment(min, max, r, Segment(mesh.p[0], mesh.p[1])); break;
    case EnvelopeCollide::TRIANGLE: filter = Functions::checkSimpleProjectorFilterTriangle(min, max, r, Triangle(mesh.p[0], mesh.p[1], mesh.p[2])); break;
    default: break;
    }
    return filter;
}

inline void ProjectorSimpleFilter::getMinMax(const PKT& p)
{
    if(p.x < min.x) min.x = p.x;
    else if(p.x > max.x) max.x = p.x;
    if(p.y < min.y) min.y = p.y;
    else if(p.y > max.y) max.y = p.y;
    if(p.z < min.z) min.z = p.z;
    else if(p.z > max.z) max.z = p.z;
}

}}


#endif // PROJECTORSIMPLEFILTER_H
