#include "SolidCriteriaGame.h"

template<>
bool SolidCriteriaGame<SolidGame>::operator ()(const SolidGame & data)
{
    if(type == TYPE_COLLISION)
        return data.type == SolidGame::TYPE_GENERIC || data.type == SolidGame::TYPE_HARD;
    return true;
}

template<>
int SolidCriteriaGame<SolidGame>::resultFunc(const SolidGame & t)
{
    return 0;
}
