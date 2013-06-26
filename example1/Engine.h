/// Author: Przemysław Bągard

#ifndef ENGINE_H
#define ENGINE_H

#include "Camera.h"
#include "Module.h"
#include "Tree/Tree4Manager.h"
#include "Player.h"
#include "Tree/ResultCollectorAllQtTemplate.h"
#include "Tree/ResultCollectorCatcherQtTemplate.h"
#include "Situation.h"

/// Program data, game world data dane programu, świata gry
/// This class is a singleton
class Engine
{
public:
    /// Game camera.
    Camera camera;

    /// Game world.
    Module module;

    /// Collision Detection Engine.
    Tree4Manager tree;

    /// Pointer to game main character.
    Player* pl;


    /** General information */

    /// Situations list.
    QVector<Situation> situations;

    /// Index of current situation from list.
    int currentSituation;

    /// Is situation sequence play mode enabled.
    bool playSequence;

    /// Current collision detection result - for all collision mode.
    ResultCollectorAllQtTemplate<SolidGame> resAll;

    /// Current collision detection result - for first (moving forward and going back) collision mode.
    ResultCollectorCatcherQtTemplate<SolidGame> resFirst;

    /// Getter for this singleton; after writing "extern Engine& en;" in .cpp file, "en." will be equivalent to "Engine::get().".
    static Engine& getInstance()
    {
      static Engine instance;
      return instance;
    }

    /// Sets gui rendering or collision detection options.
    void setGui(const GuiSettings& gui);

    /// Returns gui rendering or collision detection options.
    const GuiSettings& getGui();

    /// Save set of positions and gui settings.
    void saveSituations(const QString& filename);

    /// Load set of positions and gui settings.
    void loadSituations(const QString& filename);
private:
    Engine();
    Engine(const Engine &) {}
    Engine& operator=(const Engine&);

    /// Rendering or collision detection options.
    GuiSettings _gui;
};

#endif // ENGINE_H
