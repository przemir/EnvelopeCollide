/// Author: Przemysław Bągard

#ifndef PIERCERANGERESULT_H
#define PIERCERANGERESULT_H

#include "Geometry/Point.h"

namespace EnvelopeCollide { namespace Functions {
    using namespace EnvelopeGeometry;

/// Structure holding calculation results related with collision detection functions
struct PierceRangeResult {
    PierceRangeResult() : onCollisionTrajectory(false), vMinDistance(INFINITY), vMaxDistance(-INFINITY) {}

    /// Checks if second object is on collision trajectory (line created from vector 'vec') of first object ('vMinDistance', 'vMaxDistance' can be positive or negative)
    bool onCollisionTrajectory;

    /// Earliest collision moment ('vMinDistance*vec' is distance from object position before movement and position of this same object in collision moment)
    float vMinDistance;

    /// Latest collision moment ('vMaxDistance*vec' is distance from object position before movement and position of this same object in collision moment)
    float vMaxDistance;

    /// Movement vector
    PKT vec;

    /// Points of two objects, which will collide
    PKT vMinPoint[2], vMaxPoint[2];

    /// Vector perpendicular for earliest collision moment
    PKT vMinNorm() const { return vMinPoint[0]+vec*vMinDistance-vMinPoint[1]; }

    /// Vector perpendicular for latest collision moment
    PKT vMaxNorm() const { return vMaxPoint[0]+vec*vMaxDistance-vMaxPoint[1]; }
};

/// Merge the best result so far with new result.
inline void pierceRangeMerge(PierceRangeResult& source, const PierceRangeResult& target);

}}

namespace EnvelopeCollide { namespace Functions {

//implementation

/// Merge the best result so far with new result
/// Function assume, that 'pierceRangeResult' was initialized with values 'vMinDistance = INFINITY', 'vMaxDistance = -INFINITY'
inline void pierceRangeMerge(PierceRangeResult& source, const PierceRangeResult& target)
{
    if((source.onCollisionTrajectory = source.onCollisionTrajectory || target.onCollisionTrajectory))
    {
        if(target.vMinDistance < source.vMinDistance)
        {
            source.vMinDistance = target.vMinDistance;
            source.vMinPoint[0] = target.vMinPoint[0];
            source.vMinPoint[1] = target.vMinPoint[1];
        }
        if(target.vMaxDistance > source.vMaxDistance)
        {
            source.vMaxDistance = target.vMaxDistance;
            source.vMaxPoint[0] = target.vMaxPoint[0];
            source.vMaxPoint[1] = target.vMaxPoint[1];
        }
    }

}

}}

#endif // PIERCERANGERESULT_H
