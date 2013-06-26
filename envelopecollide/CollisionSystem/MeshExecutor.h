/// Author: Przemysław Bągard

#ifndef MESHEXECUTOR_H
#define MESHEXECUTOR_H

#include "Mesh.h"

namespace EnvelopeCollide { namespace System {

/// Gets envelope's geometric data from class T
template <typename T>
struct MeshExecutor
{
    /// Returns data of i-th element from elements list.
    virtual EnvelopeCollide::Mesh get(const T& data, int i) const = 0;

    /// Returns count of elements list.
    virtual int count(const T& data) const = 0;
};

}}

#endif // MESHEXECUTOR_H
