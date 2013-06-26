/// Author: Przemysław Bągard

#ifndef FILTER_H
#define FILTER_H

#include "Entry.h"
#include "Mesh.h"

namespace EnvelopeCollide { namespace System {

/// Pre-filter function is implemented in each class which inherits class below.
class FilterFunction
{
public:
    FilterFunction() {}

    /// Initialization of pre-filter function - used once before collision detection procedure.
    virtual inline void init(const Entry&) = 0;

    /// For each perform of collision detection procedure - is object accepted to collision detection function?
    virtual inline bool isOk(const Entry&, const Mesh&) = 0;
};

}}

#endif // FILTER_H
