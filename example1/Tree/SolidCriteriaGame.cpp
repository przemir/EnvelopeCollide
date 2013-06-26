#include "SolidCriteriaGame.h"

template<>
bool SolidCriteriaGame<SolidGame>::operator ()(const SolidGame & data)
{
    /// All shapes will take part in collision detection routine.
    if(type == TYPE_ALL)
        return true;

    /// Only normal shapes take part in collision detection routine (omit distinguished shapes).
    if(type == TYPE_NORMAL_ONLY)
        return data.type == SolidGame::TYPE_GENERIC || data.type == SolidGame::TYPE_NORMAL;

    /// Only distinguished shapes take part in collision detection routine (omit normal shapes).
    if(type == TYPE_SPECIAL_ONLY)
        return data.type == SolidGame::TYPE_SPECIAL;

    return true;
}

template<>
int SolidCriteriaGame<SolidGame>::resultFunc(const SolidGame & t)
{
    return 0;
}

