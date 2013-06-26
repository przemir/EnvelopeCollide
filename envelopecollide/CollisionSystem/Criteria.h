/// Author: Przemysław Bągard

#ifndef CRITERIA_H
#define CRITERIA_H

namespace EnvelopeCollide { namespace System {

/// Criteria based on application logic to accept/reject objects for collision detection (geometry has nothing to do there)
/// (for example we accepts walls, but we reject areas which makes our character slow)
/// Abstract class, which is used during collision detection function ('getFieldCollisions') in 'Structure'.
template<typename Data>
class Criteria
{
public:
    /// Accept/reject object
    virtual bool operator() (const Data&) = 0;

    /// To use in classess which inherits 'ResultCollector' class
    virtual int resultFunc(const Data&) = 0;
};

}}


#endif // CRITERIA_H
