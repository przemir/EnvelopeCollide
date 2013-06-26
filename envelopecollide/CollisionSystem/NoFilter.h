/// Author: Przemysław Bągard

#ifndef NOFILTER_H
#define NOFILTER_H

#include "FilterFunction.h"
#include "Entry.h"
#include "Mesh.h"

namespace EnvelopeCollide { namespace System {

/// No pre-filter function will be used
class NoFilter : public FilterFunction
{
public:
    NoFilter() {}

    inline virtual void init(const Entry&) {}
    inline virtual bool isOk(const Entry&, const Mesh&) { return true; }
};


}}

#endif // NOFILTER_H
