/// Author: Przemysław Bągard

#ifndef WRITEFIELDCONTAINER_H
#define WRITEFIELDCONTAINER_H

#include <iterator>
#include <map>

namespace EnvelopeCollide { namespace System {
/// FieldContainer have to inherit class which contains iterator (like STL or Qt)
/// Below class using STL iterators (std::map)

/// Class for holding structure fields (for example field for octree)
/// Represents area in space and holds structure's objects.
/// Class for holding dynamic objects (which can change position).
template<typename I, class Data>
class WriteFieldContainer : public std::map<I, Data>
{
public:
    typedef std::map<I, Data> ContainerType;

    /// Removes element with id 'i' from field
    void removeFieldElement(const I& i) { ContainerType::erase(i);  }

     /// Adds element with id 'i' to field
    void addFieldElement(const I& i, Data data) { ContainerType::insert(std::pair<I, Data>(i, data)); }

    /// Removes all elements in field
    void clearField() { ContainerType::clear(); }

    /// Returns element data for given iterator
    static Data getFieldElement(typename ContainerType::iterator it) { return it->second; }

    /// Returns element id for given iterator
    inline static I getFieldId(typename ContainerType::iterator it) { return it->first; }
};

}}

#endif // WRITEFIELDCONTAINER_H
