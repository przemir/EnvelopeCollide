/// Author: Przemysław Bągard

#ifndef OCTREESTRUCTURE_H
#define OCTREESTRUCTURE_H

#include "Structure.h"
#include "ResultCollector.h"
#include "Entry.h"
#include "Criteria.h"

namespace EnvelopeCollide { namespace System {

/// Structure which holds objects for collision detection - octree
template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
class OctreeStructure : public Structure<Data, Executor, Generator, Id, Filter, FieldContainer>
{
public:
    /// Structure field Id
    typedef unsigned long int treeSizeType;

    /// Object id - thanks to this id it is possible to remove element from tree (after adding object 'K' to tree, this id should be holded in object 'K')
    typedef std::pair<treeSizeType, Id> treeElementId;

    OctreeStructure();

    /// Initializes tree.
    void init(int levels, const PKT &left_down, const PKT &right_up);

    /// Allocate element to field
    treeElementId add(Data &atom);

    /// Removes object from given field.
    void remove(const treeElementId &id);

    /// Clears tree.
    void clear();

    /// Clears tree fields - in opposite to 'clear' this function does not remove Fields
    void reset();

    /// Returns count of elements holded in field with index 'fld'
    int size(treeSizeType fld);

    /// Returns range of fields - blocks with smallest size.
    /// This creates a cuboid parallel to coordinate system axis. This cuboid enclose 'entry' object.
    /// Returns extreme index coordinates for the most detailed level.
    std::pair<iPoint3D, iPoint3D> getFields(EnvelopeCollide::Entry &entry);

    /// Checks collisions within structure - determine, which fields will execute function 'getFieldCollisionsDetection'
    virtual void getCollisionsDetection(ResultCollector<Data> &result, EnvelopeCollide::Entry& entry, Criteria<Data>& criteria, Id id);
private:
    //    QVector<FieldContainer<Id, Data> > elements;
    std::vector<FieldContainer<Id, Data> > elements;  /// external - levels, internal - level elements

    /// Returns field index
    std::pair<bool, treeSizeType> getFieldForObject(const PKT& rectMin, const PKT& rectMax);

    /// Calculates bounding cuboid on model
    void objectRange(Data &atom, PKT& rectMin, PKT& rectMax);

    /// Structure containing data for the level of detail.
    struct TreeLevelTile
    {
        /// Index of the first tile for this level of detail
        int start;

        /// Count of tiles for one dimension
        int size;
    };

    /// Count of levels of detail
    int levels;

    /// Smallest size of block
    PKT tile_size;

    /// Left, down, bottom coordinates of octree area
    PKT left_down_corner;

    /// Right, up, top coordinates of octree area
    PKT right_up_corner;

    /// Information for each level of detail.
    std::vector<TreeLevelTile> level_tile;
};

}}


/// implementation
namespace EnvelopeCollide { namespace System {

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::OctreeStructure()
{
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
void OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::getCollisionsDetection(ResultCollector<Data> &result, Entry &entry, Criteria<Data> &criteria, Id id)
{
    Filter filter;
    filter.init(entry);

    std::pair<iPoint3D, iPoint3D> fields = getFields(entry);
    for(int lvl = levels-1; lvl >= 0; lvl--)
    {
        for(int lx = fields.first.x; lx <= fields.second.x; lx++)
        {
            for(int ly = fields.first.y; ly <= fields.second.y; ly++)
            {
                for(int lz = fields.first.z; lz <= fields.second.z; lz++)
                {
                    int s = level_tile[lvl].size;
                    int fld = level_tile[lvl].start + lz*s*s + ly*s + lx;
                    Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::getFieldCollisionsDetection(result, entry, criteria, id, elements[fld], filter);
                }
            }
        }
        /// Next level of detail.
        if(lvl == 0)
            break;

        fields.first /= 2;
        fields.second /= 2;
    }
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
void OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::init(int levels, const PKT &left_down, const PKT &right_up)
{
    this->levels = levels;
    int ls = 0;
    int ll = 1;
    TreeLevelTile til;
    for(int l1 = 0; l1 < levels; l1++)
    {
        til.start = ls;
        til.size = ll;

        level_tile.push_back(til);
        ls += ll*ll*ll;
        ll *= 2;
    }

    int el = 0;
    int el_level = 1;
    for(int l1 = 0; l1 < levels; l1++)
    {
        el += el_level;
        el_level *= 8;
    }

    for(int l1 = 0; l1 < el; l1++)
        elements.push_back(FieldContainer<Id, Data>());

    /// Sets octree area.
    left_down_corner = left_down;
    right_up_corner = right_up;

    tile_size = right_up_corner-left_down_corner;
    for(int l1 = 0; l1 < levels-1; l1++)
        tile_size /= 2.0f;
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
void OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::remove(const treeElementId &id)
{
    elements[id.first].removeFieldElement(id.second);
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
inline void OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::objectRange(Data &data, PKT& rectMin, PKT& rectMax)
{
    EnvelopeCollide::Mesh checked = Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::executor.get(data, 0);
    rectMin = checked.p[0];
    rectMax = checked.p[0];
    do
    {
        int count = Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::executor.count(data);
        for(int l1 = 0; l1 < count; ++l1)
        {
            checked = Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::executor.get(data, l1);
            for(int l2 = 0; l2 < checked.pCount(); ++l2)
            {
                if(checked.p[l2].x - checked.r < rectMin.x) rectMin.x = checked.p[l2].x - checked.r;
                if(checked.p[l2].y - checked.r < rectMin.y) rectMin.y = checked.p[l2].y - checked.r;
                if(checked.p[l2].z - checked.r < rectMin.z) rectMin.z = checked.p[l2].z - checked.r;
                if(checked.p[l2].x + checked.r > rectMax.x) rectMax.x = checked.p[l2].x + checked.r;
                if(checked.p[l2].y + checked.r > rectMax.y) rectMax.y = checked.p[l2].y + checked.r;
                if(checked.p[l2].z + checked.r > rectMax.z) rectMax.z = checked.p[l2].z + checked.r;
            }
        }
    }
    while(Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::generator.next(data));
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
typename OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::treeElementId OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::add(Data &atom)
{
    // min, max
    Data data = atom;
    PKT rectMin, rectMax;
    if(!Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::generator.first(data, atom))
        return treeElementId(0, Id());

    // Element have to be related with some envelope
    if(Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::executor.count(data) <= 0)
        return treeElementId(0, Id());

    Id id = Structure<Data, Executor, Generator, Id, Filter, FieldContainer>::generator.generateId(data);

    objectRange(data, rectMin, rectMax);

    // insertion
    std::pair<bool, treeSizeType> val = getFieldForObject(rectMin, rectMax);
    if(val.first)
    {
        elements[val.second].addFieldElement(id, atom);
        return treeElementId(val.second, id);
    }

    return treeElementId(0, Id());
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
int OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::size(treeSizeType fld)
{
    return elements[fld].size();
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
void OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::clear()
{
    elements.clear();
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
void OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::reset()
{
    for(int i = 0; i < elements.size(); ++i)
        elements[i].clear();
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
std::pair<bool, typename OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::treeSizeType> OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::getFieldForObject(const PKT& rectMin, const PKT& rectMax)
{
    PKT left_down = left_down_corner;
    PKT right_up = right_up_corner;
    PKT mid;
    int li;
    int ix = 0;      // Blocks coordinates in octree.
    int iy = 0;
    int iz = 0;
    for(int lvl = 0; lvl < levels; lvl++)
    {
        li = level_tile[lvl].start;
        if(lvl == levels-1) // Inserts a element.
        {
            //int i = li + iy*lw + ix;
            int s = level_tile[lvl].size;
            int i = li + iz*s*s + iy*s + ix;
            return std::pair<bool, treeSizeType>(true, i);
        }
        else
        {
            mid = (left_down + right_up) / 2.0f;

            bool left = false;
            bool right = false;
            bool up = false;
            bool down = false;
            bool top = false;
            bool bottom = false;

            int valX;  // 0 - on mid, 1 - on left or on right side, 2 - is intersected with line
            int valY;
            int valZ;


            if(rectMin.x < mid.x) left = true;
            if(rectMax.x > mid.x) right = true;
            valX = (int)left + (int)right;


            if(rectMin.y < mid.y) bottom = true;
            if(rectMax.y > mid.y) top = true;
            valY = (int)top + (int)bottom;

            if(rectMin.z < mid.z) down = true;
            if(rectMax.z > mid.z) up = true;
            valZ = (int)down + (int)up;

            if(valX == 1 && valY == 1 && valZ == 1) // go to next level of detail
            {
                ix = ix*2 + (int)right;
                if(right == false) right_up.x = mid.x;
                else left_down.x = mid.x;

                iy = iy*2 + (int)top;
                if(top == false) right_up.y = mid.y;
                else left_down.y = mid.y;

                iz = iz*2 + (int)up;
                if(up == false) right_up.z = mid.z;
                else left_down.z = mid.z;
            }
            else    // Inserts a element.
            {
                //int i = li + iy*lw + ix;
                int s = level_tile[lvl].size;
                int i = li + iz*s*s + iy*s + ix;
                return std::pair<bool, treeSizeType>(true, i);
            }
        }
    }
    return std::pair<bool, treeSizeType>(false, treeSizeType());
}

template<typename Data, class Executor, class Generator, typename Id, class Filter, template<typename Id, class T> class FieldContainer >
std::pair<iPoint3D, iPoint3D> OctreeStructure<Data, Executor, Generator, Id, Filter, FieldContainer>::getFields(EnvelopeCollide::Entry& entry)
{
    PKT minr, maxr;
    minr = entry.mesh.p[0];
    maxr = entry.mesh.p[0];
    for(int l1 = 0; l1 < entry.mesh.pCount(); l1++)
    {
        minr.x = min(minr.x, entry.mesh.p[l1].x + entry.vec.x*entry.back_max - entry.mesh.r);
        minr.x = min(minr.x, entry.mesh.p[l1].x + entry.vec.x*entry.front_max - entry.mesh.r);
        minr.y = min(minr.y, entry.mesh.p[l1].y + entry.vec.y*entry.back_max - entry.mesh.r);
        minr.y = min(minr.y, entry.mesh.p[l1].y + entry.vec.y*entry.front_max - entry.mesh.r);
        minr.z = min(minr.z, entry.mesh.p[l1].z + entry.vec.z*entry.back_max - entry.mesh.r);
        minr.z = min(minr.z, entry.mesh.p[l1].z + entry.vec.z*entry.front_max - entry.mesh.r);

        maxr.x = max(maxr.x, entry.mesh.p[l1].x + entry.vec.x*entry.back_max + entry.mesh.r);
        maxr.x = max(maxr.x, entry.mesh.p[l1].x + entry.vec.x*entry.front_max + entry.mesh.r);
        maxr.y = max(maxr.y, entry.mesh.p[l1].y + entry.vec.y*entry.back_max + entry.mesh.r);
        maxr.y = max(maxr.y, entry.mesh.p[l1].y + entry.vec.y*entry.front_max + entry.mesh.r);
        maxr.z = max(maxr.z, entry.mesh.p[l1].z + entry.vec.z*entry.back_max + entry.mesh.r);
        maxr.z = max(maxr.z, entry.mesh.p[l1].z + entry.vec.z*entry.front_max + entry.mesh.r);
    }

    std::pair<iPoint3D, iPoint3D> result;
    minr.x = (minr.x-left_down_corner.x)/tile_size.x;
    minr.y = (minr.y-left_down_corner.y)/tile_size.y;
    minr.z = (minr.z-left_down_corner.z)/tile_size.z;
    maxr.x = (maxr.x-left_down_corner.x)/tile_size.x;
    maxr.y = (maxr.y-left_down_corner.y)/tile_size.y;
    maxr.z = (maxr.z-left_down_corner.z)/tile_size.z;

    result.first = iPoint3D(minr.x, minr.y, minr.z);
    if(isZero(minr.x - (float)result.first.x)) result.first.x--;
    if(isZero(minr.y - (float)result.first.y)) result.first.y--;
    if(isZero(minr.z - (float)result.first.z)) result.first.z--;
    if(result.first.x < 0) result.first.x = 0;
    if(result.first.y < 0) result.first.y = 0;
    if(result.first.z < 0) result.first.z = 0;
    result.second = iPoint3D(maxr.x, maxr.y, maxr.z);
    if(result.second.x >= level_tile[levels-1].size) result.second.x = level_tile[levels-1].size-1;
    if(result.second.y >= level_tile[levels-1].size) result.second.y = level_tile[levels-1].size-1;
    if(result.second.z >= level_tile[levels-1].size) result.second.z = level_tile[levels-1].size-1;

    return result;
}



}}


#endif // OCTREESTRUCTURE_H
