/// Author: Przemysław Bągard

#ifndef GAMEMESHEXECUTOR_H
#define GAMEMESHEXECUTOR_H

#include "SolidGame.h"
#include "CollisionSystem/MeshExecutor.h"

/// Gets envelope's geometric data from class T.
template <typename T>
struct GameMeshExecutor : public EnvelopeCollide::System::MeshExecutor<T>
{
    /// Returns data of i-th element from elements list.
    virtual EnvelopeCollide::Mesh get(const T& data, int i) const;

    /// Returns count of elements list.
    virtual int count(const T& data) const;
};

#endif // GAMEMESHEXECUTOR_H
