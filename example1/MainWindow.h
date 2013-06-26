/// Author: Przemysław Bągard

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTreeWidgetItem"
#include "Object.h"
#include "Situation.h"

namespace Ui {
class MainWindow;
}

/// Program GUI
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    /// Returns tree element at the mouse position.
    QTreeWidgetItem* getTreeItemId();

    /// Returns object data at the mouse position.
    QPair<float, Object::Id> getItemData();
public slots:
    /// gui -> engine 'GuiSettings'
    void optionsFromGui();

    /// engine 'Player position' -> gui
    void playerPosToGui();

    /// gui -> engine 'Player position'
    void playerPosFromGui();

    /// Sets player position to (0,0,0).
    void centerPlayer();

    /// Create collision detection results list.
    void generateCollisionTree();

    /// Reaction to changed collision interpretation mode combobox value.
    void collisionModeChanged();

    /// Reaction to changed player shape combobox value.
    void collisionMeshChanged();

    /// About this program.
    void about();

    /// Loads situation sequence from file.
    void loadSituation();

    /// Saves situation sequence to file.
    void saveSituation();

    /// Records current situation and add it to situation sequence.
    void captureSituation();

    /// Plays/Stops playing situation sequence.
    void playSequence();

    /// Backs to the first situation from the sequence.
    void toFirstSituation();

    /// This situation time is up.
    void sequenceTimeout();

    /// Enters next situation from the sequence.
    void nextSituation();

    /// Enters previous situation from the sequence.
    void previousSituation();
private:
    /// Blocks signals of widgets related with properties.
    void guiBlock(bool blocked);

    /// Blocks signals of widgets related with character position.
    void playerPosBlock(bool blocked);

    /// situation settings -> gui
    void situationToGui(const Situation &situation);

    /// widgets
    Ui::MainWindow *ui;

    /// Sequence player timer.
    QTimer* timer;
};

#endif // MAINWINDOW_H
