/// Author: Przemysław Bągard

#ifndef COLLISIONFILTER_H
#define COLLISIONFILTER_H

#include "PierceRangeResult.h"
#include "Geometry/Plane.h"
#include "Geometry/Point.h"
#include "Geometry/math.h"
#include "Geometry/Projector.h"
#include "Geometry/Segment.h"
#include "Geometry/Triangle.h"
#include "Mesh.h"

/// Pre-filter functions
namespace EnvelopeCollide { namespace Functions {

/// Returns projector which first vector is parallel to 'vec' vector
Projector makeCubeProjectorFromMoveVector(PKT vec);

/// Tafla (local definition) - space bounded by two parallel planes
/// Tafle intersection method - for three Tafles (perpendicular to each other) checks if figure is inside or intersects with each tafla.
/// Although intersection of taflas creates a cuboid, this is not intersection of figure with cuboid
/// (every figure which collide with cuboid, also collide with intersection of taflas; collisions with cuboid rejects more object,
///  hovewer there are slower (because it is necessary to calculate exact place where figure intersects cuboid)).

/// Updates 'min' and 'max' point in projector 'proj' coordinate system for given point 'p'.
/// This is used to create bounding box for each point from the set. Every 'p' point can extend this box;
/// ATTENTION! 'min' and 'max' have to be initialized properly (with any point from set or with values 'min=INFINITY', 'max=-INFINITY')
void getProjectorMinMax(const Projector& proj, PKT p, PKT& min, PKT& max);

/// Pre-filter function using tafle intersection method method with planes parallel to projector coordinate system.
bool checkMoveVectorProjectorFilterPoint(const Projector& proj, PKT min, PKT max, const float& r, const PKT& p1);

/// Pre-filter function using tafle intersection method method with planes parallel to projector coordinate system.
bool checkMoveVectorProjectorFilterSegment(const Projector& proj, PKT min, PKT max, const float& r, const Segment& s);

/// Pre-filter function using tafle intersection method method with planes parallel to projector coordinate system.
bool checkMoveVectorProjectorFilterTriangle(const Projector& proj, PKT min, PKT max, const float& r, const Triangle& s);

/// Pre-filter function using tafle intersection method method with planes parallel to global coordinate system.
bool checkSimpleProjectorFilterPoint(PKT min, PKT max, const float& r, const PKT& s);

/// Pre-filter function using tafle intersection method method with planes parallel to global coordinate system.
bool checkSimpleProjectorFilterSegment(PKT min, PKT max, const float& r, const Segment& s);

/// Pre-filter function using tafle intersection method method with planes parallel to global coordinate system.
bool checkSimpleProjectorFilterTriangle(PKT min, PKT max, const float& r, const Triangle& s);

/// Creates bounding sphere for 'source' object quickly. 'target' reference store function result.
void createBoundingSphereSimple(Mesh& target, const Mesh& source);

/// Creates bounding sphere for 'source' object accurately. 'target' reference store function result.
void createBoundingSphere(Mesh& target, const Mesh& source);

}}

#endif // COLLISIONFILTER_H
