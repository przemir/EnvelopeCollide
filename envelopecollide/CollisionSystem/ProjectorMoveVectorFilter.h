/// Author: Przemysław Bągard

#ifndef PROJECTORMOVEVECTORFILTER_H
#define PROJECTORMOVEVECTORFILTER_H

#include "FilterFunction.h"
#include "Entry.h"
#include "Mesh.h"
#include "CollisionFilter.h"
#include "Geometry/Projector.h"

namespace EnvelopeCollide { namespace System {

/// Pre-filter class using tafle intersection method method with planes parallel to projector coordinate system (definition of taflas in "CollisionFilter.h").
class ProjectorMoveVectorFilter : public FilterFunction
{
public:
    ProjectorMoveVectorFilter();

    inline virtual void init(const Entry& entry);
    inline virtual bool isOk(const Entry& entry, const Mesh& mesh);

    /// Projector which transform global coordinates to another coordinate system.
    Projector proj;

    /// Returns left, down, bottom cuboid coordinates
    PKT min;

    /// Returns right, up, top cuboid coordinates
    PKT max;
};

}}


namespace EnvelopeCollide { namespace System {

inline ProjectorMoveVectorFilter::ProjectorMoveVectorFilter() : proj(Projector(PKT0, Punkt(1, 0, 0), Punkt(0,1,0)))
{
}

inline void ProjectorMoveVectorFilter::init(const EnvelopeCollide::Entry &entry)
{
    proj = EnvelopeCollide::Functions::makeCubeProjectorFromMoveVector(entry.vec);
    min = proj.transform(entry.mesh.p[0]);
    max = min;
    for(int l1 = 0; l1 <= entry.mesh.pCount(); l1++)
    {
        Functions::getProjectorMinMax(proj, entry.mesh.p[l1]+entry.vec*entry.back_max, min, max);
        Functions::getProjectorMinMax(proj, entry.mesh.p[l1]+entry.vec*entry.front_max, min, max);
    }
}

inline bool ProjectorMoveVectorFilter::isOk(const EnvelopeCollide::Entry &entry, const EnvelopeCollide::Mesh &mesh)
{
    bool filter = false;
    float r = entry.mesh.r + mesh.r + EnvelopeCollide::COLLISION_MIN_R;
    switch(mesh.type)
    {
    case EnvelopeCollide::POINT: filter = Functions::checkMoveVectorProjectorFilterPoint(proj, min, max, r, mesh.p[0]); break;
    case EnvelopeCollide::LINE: filter = Functions::checkMoveVectorProjectorFilterSegment(proj, min, max, r, Segment(mesh.p[0], mesh.p[1])); break;
    case EnvelopeCollide::TRIANGLE: filter = Functions::checkMoveVectorProjectorFilterTriangle(proj, min, max, r, Triangle(mesh.p[0], mesh.p[1], mesh.p[2])); break;
    default: break;
    }
    return filter;
}

}}


#endif // PROJECTORMOVEVECTORFILTER_H
