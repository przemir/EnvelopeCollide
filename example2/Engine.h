/// Author: Przemysław Bągard

#ifndef ENGINE_H
#define ENGINE_H

#include "Camera.h"
#include "Module.h"
#include "Controllers.h"
#include "Tree/Tree4Manager.h"
#include "Player.h"
#include "Moves.h"

/// Program data, game world data dane programu, świata gry
/// This class is a singleton
class Engine
{
public:
    /// Game camera.
    Camera camera;

    /// Game world.
    Module module;

    /// Handles keyboard and manage program keys binding.
    Controllers control;

    /// Collision Detection Engine.
    Tree4Manager tree;

    /// Pointer to game main character.
    Player* pl;

    /// Pointer to sequence of moves.
    Moves* moves;

    /// Getter for this singleton; after writing "extern Engine& en;" in .cpp file, "en." will be equivalent to "Engine::get().".
    static Engine& getInstance()
    {
      static Engine instance;
      return instance;
    }

    /// Calculates direction radius from movement vector.
    float calculateRad(PKT norm);

    /// Character movement process - character movement, collisions with obstacles, entering/exiting/staying in object.
    void processMove(float elapsed_time_with_direction);

private:
    Engine();
    Engine(const Engine &) {}
    Engine& operator=(const Engine&);
};

#endif // ENGINE_H
