/// Author: Przemysław Bągard

#ifndef BOUNDINGSPHEREFILTER_H
#define BOUNDINGSPHEREFILTER_H

#include "FilterFunction.h"
#include "Entry.h"
#include "Mesh.h"
#include "CollisionDetection.h"
#include "CollisionFilter.h"
#include "PierceRangeResult.h"

namespace EnvelopeCollide { namespace System {

/// Pre-filter function based on bounding sphere - variant in which sphere is calculated accurately
class BoundingSphereFilter : public FilterFunction
{
public:
    BoundingSphereFilter() {}

    inline virtual void init(const Entry& entry);
    inline virtual bool isOk(const Entry& entry, const Mesh& mesh);

    /// Bounding sphere for object used by pre-filter function
    Mesh entryMesh;
};


}}


/// implementation
namespace EnvelopeCollide { namespace System {

inline void BoundingSphereFilter::init(const EnvelopeCollide::Entry &entry)
{
    EnvelopeCollide::Functions::createBoundingSphere(entryMesh, entry.mesh);
}

inline bool BoundingSphereFilter::isOk(const EnvelopeCollide::Entry &entry, const EnvelopeCollide::Mesh &mesh)
{
    Mesh simpleMesh;
    EnvelopeCollide::Functions::createBoundingSphere(simpleMesh, mesh);
    EnvelopeCollide::Functions::PierceRangeResult result = EnvelopeCollide::Functions::pierceRangePointPoint(entryMesh.p[0], entry.vec, simpleMesh.p[0], entryMesh.r+simpleMesh.r+0.0001f);
    return result.onCollisionTrajectory && result.vMinDistance <= entry.front_max && result.vMaxDistance >= entry.back_max;
}

}}



#endif // BOUNDINGSPHEREFILTER_H
