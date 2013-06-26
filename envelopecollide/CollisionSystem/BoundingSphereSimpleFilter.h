/// Author: Przemysław Bągard

#ifndef BOUNDINGSPHERESIMPLEFILTER_H
#define BOUNDINGSPHERESIMPLEFILTER_H

#include "FilterFunction.h"
#include "Entry.h"
#include "Mesh.h"
#include "CollisionDetection.h"
#include "CollisionFilter.h"
#include "PierceRangeResult.h"

namespace EnvelopeCollide { namespace System {

/// Pre-filter function based on bounding sphere - variant in which sphere is calculated quickly
class BoundingSphereSimpleFilter : public FilterFunction
{
public:
    BoundingSphereSimpleFilter() {}

    inline virtual void init(const Entry& entry);
    inline virtual bool isOk(const Entry& entry, const Mesh& mesh);

    /// Bounding sphere for object used by pre-filter function
    Mesh entryMesh;
};


}}


/// implementation
namespace EnvelopeCollide { namespace System {

inline void BoundingSphereSimpleFilter::init(const EnvelopeCollide::Entry &entry)
{
    EnvelopeCollide::Functions::createBoundingSphereSimple(entryMesh, entry.mesh);
}

inline bool BoundingSphereSimpleFilter::isOk(const EnvelopeCollide::Entry &entry, const EnvelopeCollide::Mesh &mesh)
{
    Mesh simpleMesh;
    EnvelopeCollide::Functions::createBoundingSphereSimple(simpleMesh, mesh);
    EnvelopeCollide::Functions::PierceRangeResult result = EnvelopeCollide::Functions::pierceRangePointPoint(entryMesh.p[0], entry.vec, simpleMesh.p[0], entryMesh.r+simpleMesh.r+0.0001f);
    return result.onCollisionTrajectory && result.vMinDistance <= entry.front_max && result.vMaxDistance >= entry.back_max;
}

}}


#endif // BOUNDINGSPHERESIMPLEFILTER_H
