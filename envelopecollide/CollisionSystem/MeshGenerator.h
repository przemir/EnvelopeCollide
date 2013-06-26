/// Author: Przemysław Bągard

#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

namespace EnvelopeCollide { namespace System {

/// Gets physical models from class T
template <typename T, typename Id>
struct MeshGenerator
{
    /// iteration
    /// Returns first physical model for T element.
    /// Returns true, if physical model exists, otherwise element not holds envelopes (class 'Mesh' objects).
    virtual bool first(T& data, const T& source) = 0;

    /// Returns next physical model for T element.
    /// Returns false, if there is no more physical objects, otherwise returns true and in 'data' variable store object information.
    virtual bool next(T& data) = 0;

    /// If application logic criteria (not geometry) rejects this object, then whole category of physical models will be skipped.
    /// 'data' variable indicate to object's element, which will be considered as next (this element will also be checked by this function)
    /// Write 'skip { next(data); }' if you don't use this
    virtual bool skip(T& data) = 0;

    /// Generate object id (unique within a structure field).
    /// Id is used to remove object or check whether we check collision for object with itself.
    virtual Id generateId(const T& source) = 0;

    /// Action to do before collision detection.
    /// This can be for example if we check collision for object with itself.
    virtual bool allow(const Id& id_source, const Id& id_current) = 0;
};

}}

#endif // MESHGENERATOR_H
