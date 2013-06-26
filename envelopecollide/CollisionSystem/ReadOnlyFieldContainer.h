/// Author: Przemysław Bągard

#ifndef READONLYFIELDCONTAINER_H
#define READONLYFIELDCONTAINER_H

#include <vector>

namespace EnvelopeCollide { namespace System {
/// FieldContainer have to inherit class which contains iterator (like STL or Qt)
/// Below class using STL iterators (std::vector)

/// Class for holding structure fields (for example field for octree)
/// Represents area in space and holds structure's objects.
/// Objects are inserted at the beginning and after that field can not be modified.
/// (with the exception of 'clearField' function).
/// Class for holding static objects (which can not change position).
template<typename I, class Data>
class ReadOnlyFieldContainer : public std::vector<Data>
{
public:
    typedef std::vector<Data> ContainerType;

    /// Removes element with id 'i' from field - this class not support this method
    inline void removeFieldElement(const I& i) {}

    /// Adds element with id 'i' to field
    inline void addFieldElement(const I&, Data data) { ContainerType::push_back(data); }

    /// Removes all elements in field
    inline void clearField() { ContainerType::clear(); }

    /// Returns element data for given iterator
    inline static Data getFieldElement(typename ContainerType::iterator it) { return (*it); }

    /// Returns element id for given iterator
    inline static I getFieldId(typename ContainerType::iterator) { return I(); }
};

}}

#endif // READONLYFIELDCONTAINER_H
