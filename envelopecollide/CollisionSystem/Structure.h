/// Author: Przemysław Bągard

#ifndef STRUCTURE_H
#define	STRUCTURE_H

#include "Geometry/Projector.h"
#include "ResultCollector.h"
#include "Entry.h"
#include "Result.h"
#include "vector"
#include "IsDerivedFrom.h"
#include "MeshGenerator.h"
#include "MeshExecutor.h"
#include "Criteria.h"
#include "FilterFunction.h"

namespace EnvelopeCollide { namespace System {


/// Using TreeIsDerivedFrom we can force user to make sure that class inherits from concrete class (thanks to that user will know what methods implements).
/// Data - objects data which we want to store - vary depending on a program
/// Exectur - parameter which extracts geometric data (envelopes) from parameter 'Data'.
/// Generator - we don't have to insert one object envelope for each object - we can insert one object which represents them all, and iterate trought them using Generator parameter.
///             It is usefull especially for dynamic 'Structure', where objects can be modified in each frame.
/// Id        - unique within a field number/struct, used for example to avoid checking whether object collide with itself.
/// Filter    - pre-filter function used by this structure
/// FieldContainer - grouping objects within concrete field (for example list for static objects or map for dynamic)
/// Class which inherits from class below is collision detection system for appliaction)
template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
class Structure : public TreeIsDerivedFrom<Executor, MeshExecutor<Data> >, public TreeIsDerivedFrom<Generator, MeshGenerator<Data, Id> >, public TreeIsDerivedFrom<Filter, FilterFunction >
{
public:
    Structure();

    /// Checks collisions within structure - determine, which fields will execute function 'getFieldCollisionsDetection' - to define in a class that inherits this method
    virtual void getCollisionsDetection(ResultCollector<Data> &result, EnvelopeCollide::Entry& entry, Criteria<Data>& criteria, Id id) = 0;

    /// Checks collisions within concrete field.
    void getFieldCollisionsDetection(ResultCollector<Data> &result, EnvelopeCollide::Entry& entry, Criteria<Data>& criteria, Id id, FieldContainer<Id, Data>&, Filter& filter);

    /// Access to 'Executor' functions.
    Executor executor;
    /// Access to 'Generator' functions.
    Generator generator;
};

}}

/// implementation
namespace EnvelopeCollide { namespace System {

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::Structure()
{
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
void Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::getFieldCollisionsDetection(ResultCollector<Data> &result, EnvelopeCollide::Entry &entry, Criteria<Data> &criteria, Id id, FieldContainer<Id, Data>& field, Filter &filter)
{
    Data data;
    Functions::PierceRangeResult chosen, candidate;
    typename FieldContainer<Id, Data>::ContainerType::iterator it = field.begin();
    for(; it != field.end();++it)
    {
        if(!generator.allow(id, FieldContainer<Id, Data>::getFieldId(it)))
            continue;

        data = FieldContainer<Id, Data>::getFieldElement(it);
        bool done = generator.first(data, data);

        while(done)
        {
            /// Application logic filters to accept/reject object
            if(!criteria(data))
            {
                done = generator.skip(data);
                continue;
            }
            /// For each physical model's envelopes
            int count = executor.count(data);
            chosen = Functions::PierceRangeResult();//.onCollisionTrajectory = false;
            for(int l1 = 0; l1 < count; ++l1)
            {
                EnvelopeCollide::Mesh d = executor.get(data, l1);
                if(filter.isOk(entry, d))
                {
                    candidate = entry.collision(d);
                    if(candidate.onCollisionTrajectory)
                        pierceRangeMerge(chosen, candidate);
                }
            }
            result(entry, criteria, chosen, data);
            done = generator.next(data);
        }
    }
}


}}

#endif	/* STRUCTURE_H */

