#include "Engine.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>

/** Global 'Engine' instance - store information within whole program. */
Engine& en = Engine::getInstance();

/**
  After writing:

extern Engine& en;

  in .cpp file, "en." will be equivalent to "Engine::get().".
**/

Engine::Engine() : currentSituation(0), playSequence(false)
{
}


void Engine::saveSituations(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (int i = 0; i < situations.size(); ++i) {
        out << situations[i];
    }
}

void Engine::loadSituations(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    situations.clear();

    QTextStream in(&file);
    in.setCodec("UTF-8");
    int l = 0;
    /// Checks whether sequence format is valid
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList tokens = line.split(" ", QString::SkipEmptyParts);
        if(tokens.count() != 19)
        {
            QMessageBox::critical(NULL, "Bad sequence format", "Sequence cannot be loaded. ");
            return;
        }
        ++l;
    }
    in.seek(0);

    for (int i = 0; i < l; ++i) {
        Situation situation;
        in >> situation;
        situations.push_back(situation);
    }
}

void Engine::setGui(const GuiSettings &gui)
{
    _gui = gui;
}

const GuiSettings &Engine::getGui()
{
    return _gui;
}
