/// Author: Przemysław Bągard

#ifndef RESULTCOLLECTOR_H
#define RESULTCOLLECTOR_H

#include "Entry.h"
#include "Criteria.h"
#include "PierceRangeResult.h"

namespace EnvelopeCollide { namespace System {

/// Class which collects collision detection results and create events (like object enters second object in time...).
/// Abstract class, which is used during 'getFieldCollisions' function in 'Structure'.
template<typename T>
class ResultCollector
{
public:
    virtual void operator() (const EnvelopeCollide::Entry&, EnvelopeCollide::System::Criteria<T>&, EnvelopeCollide::Functions::PierceRangeResult&, const T&) = 0;
};

}}

#endif // RESULTCOLLECTOR_H
