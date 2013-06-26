/// Author: Przemysław Bągard

#ifndef SOLIDCRITERIAGAME_H
#define SOLIDCRITERIAGAME_H

#include "SolidGame.h"
#include "CollisionSystem/Criteria.h"

/// Application logic criteria
/// Criteria based on application logic to accept/reject objects for collision detection (geometry has nothing to do there)
/// (for example we accepts walls, but we reject areas which makes our character slow)
template<typename Data>
class SolidCriteriaGame : public EnvelopeCollide::System::Criteria<Data>
{
public:
    /// Type of criteria
    enum TYPE {
        /// 'hard' shapes like walls.
        TYPE_COLLISION,

        /// 'soft' shapes - special handling for collision between character and object.
        TYPE_ACTION
    } type;

    /// Accept/reject object.
    virtual bool operator() (const Data&);

    /// To use in classess which inherits 'ResultCollector' class.
    virtual int resultFunc(const Data&);
};

#endif // SOLIDCRITERIAGAME_H
