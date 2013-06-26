/// Author: Przemysław Bągard

#ifndef RESULTCOLLECTORALLQTTEMPLATE_H
#define RESULTCOLLECTORALLQTTEMPLATE_H

#include "CollisionSystem/ResultCollector.h"
#include "CollisionSystem/Result.h"
#include "QMultiMap"


/// Gather all collision events (object enters/exits another object or object is inside another object) sorted by collision moment.
template<typename T>
class ResultCollectorAllQtTemplate : public EnvelopeCollide::System::ResultCollector<T>, public QMultiMap<float, EnvelopeCollide::Result<T> >
{
public:
    ResultCollectorAllQtTemplate() {}

    /// Operator called after collision detection procedure.
    virtual void operator() (const EnvelopeCollide::Entry&, EnvelopeCollide::System::Criteria<T>&, EnvelopeCollide::Functions::PierceRangeResult&, const T&);
};



/// implementation

template<typename T>
void ResultCollectorAllQtTemplate<T>::operator ()(const EnvelopeCollide::Entry & entry, EnvelopeCollide::System::Criteria<T> &, EnvelopeCollide::Functions::PierceRangeResult &chosen, const T &atom)
{
    /// Procedure (for 'ResultCollectorAllQtTemplate' class) assumes that 'entry.back_max == 0.0f'.
    Q_ASSERT(entry.back_max == 0.0f);
    if(chosen.onCollisionTrajectory && chosen.vMaxDistance >= 0.0f)
    {
        /// Object is on collision trajectory and object isn't behind second object.
        EnvelopeCollide::Result<T> detected;
        detected.data = atom;
        if(chosen.vMinDistance <= 0.0f)
        {
            /// Object is inside another object (before moving).
            detected.mode = EnvelopeCollide::EINSIDE;
            detected.norm = chosen.vMinNorm();
            QMultiMap<float, EnvelopeCollide::Result<T> >::insert(chosen.vMinDistance, detected);
        }
        else if(chosen.vMinDistance <= entry.front_max)
        {
            /// Object enters another object.
            detected.mode = EnvelopeCollide::EENTER;
            detected.norm = chosen.vMinNorm();
            QMultiMap<float, EnvelopeCollide::Result<T> >::insert(chosen.vMinDistance, detected);
        }
        if(chosen.vMaxDistance <= entry.front_max)
        {
            /// Object exits another object.
            detected.mode = EnvelopeCollide::EEXIT;
            detected.norm = chosen.vMaxNorm();
            QMultiMap<float, EnvelopeCollide::Result<T> >::insert(chosen.vMaxDistance, detected);
        }
    }
}


#endif // RESULTCOLLECTORALLQTTEMPLATE_H
