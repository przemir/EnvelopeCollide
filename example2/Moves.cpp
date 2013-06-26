#include "Moves.h"
#include "Engine.h"
#include "GLView.h"
#include "QDebug"
#include "QFile"
#include <QStringList>
#include <QMessageBox>

extern Engine& en;

Moves::Moves(GLView* view) : time(0.0f), view(view)
{
}

void Moves::loadError(int lineNumber)
{
    moves.clear();
    QString er = QString("Błąd w linii: %1").arg(lineNumber);
    QMessageBox::critical(view, "Niepoprawny format pliku animacji", "Animacja nie może zostać wczytana. " + er);
}

float Moves::getEventCurrentTime()
{
    return time;
}

float Moves::getEventCurrentMaxTime()
{
    return moves[current].time;
}

bool Moves::loadFromFile(const QString &name)
{
    /// Removes current moves from sequence.
    moves.clear();
    current = 0;
    time = 0.0f;
    start = Punkt();

    /// Loads from file.
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(view, "Plik formatu animacji nie istnieje", "Animacja nie może zostać wczytana. ");
        return false;
    }

    QTextStream in(&file);

    /// Split text to lines.
    /// Reads line-by-line.
    /// Each line is split to tokens.
    QString line;
    QStringList data;

    int lineNumber = 0;
    while (!in.atEnd()) {
        ++lineNumber;
        line = in.readLine();
        data = line.split(" ");

        if(data.size() == 0 || (data.size() == 1 && data[0] == ""))
            continue;

        /// First letter specifies move type: f - forward, b - backward, s - stand, o - rotate
        Move move;
        if(data[0] == "f")
            move.state = STATE_GO_FRONT;
        else if(data[0] == "b")
            move.state = STATE_GO_BACK;
        else if(data[0] == "s")
            move.state = STATE_STAND;
        else if(data[0] == "o")
            move.state = STATE_ROTATE;
        else
        {
            loadError(lineNumber);
            return false;
        }

        /// Loads data after letter (duration time, rotation angle). If there is no such data, gets default value.
        if(data.size() > 1)
            move.time = data[1].toFloat();
        else
            move.time = 1.0f;
        if(data.size() > 2)
            move.rad = data[2].toFloat();
        else
            move.rad = 0.0;

        /// Adds move to sequence of moves.
        moves.push_back(move);
    }

    return true;
}

void Moves::addMove(const Moves::Move &move)
{
    moves.push_back(move);
}

int Moves::getCurrent() const
{
    return current;
}

void Moves::applyNext()
{
    if(current < moves.size())
    {
        /// There are still moves in sequence.

        /// Sets animation state based on current move type.
        if(moves[current].state == STATE_STAND)
        {
            if(en.pl->model.getCurrentState() != ANIMATION_STAND)
                en.pl->model.setAnimationState(ANIMATION_STAND);
        }
        else if(moves[current].state == STATE_ROTATE)
        {
            startRad = en.pl->radXZ;
            if(en.pl->model.getCurrentState() != ANIMATION_STAND)
                en.pl->model.setAnimationState(ANIMATION_STAND);
        }
        else
        {
            if(en.pl->model.getCurrentState() != ANIMATION_MOVE)
                en.pl->model.setAnimationState(ANIMATION_MOVE);
        }
    }
}

void Moves::firstMove()
{
    current = 0;
    time = 0.0f;
    en.pl->pos = start;
    en.pl->radXZ = 0;
    applyNext();
    en.processMove(0.0f);
}

void Moves::playAnimation(double elapsed_time)
{
    while(current < moves.size())
    {
        Move move = moves[current];

        /// Time calculations.
        double etime = elapsed_time;
        if(time + elapsed_time > move.time)
            etime = move.time - time;
        time += elapsed_time;
        double stateTime = time;
        if(stateTime > move.time)
            stateTime = move.time;

        /// Handles moves.
        if(move.state == STATE_ROTATE)
        {
            if(move.time != 0.0)
            {
                double percent = stateTime / move.time;
                en.pl->radXZ = startRad + move.rad * percent;
            }
            en.processMove(0.0f);
        }
        else if(move.state == STATE_STAND)
        {
            en.processMove(0.0f);
        }
        else
        {
            float elapsed_time_with_direction = etime;
            if(move.state == STATE_GO_BACK)
                elapsed_time_with_direction = -elapsed_time_with_direction;
            en.processMove(elapsed_time_with_direction);
        }

        if(time >= move.time)
        {
            /// Current move is finished.
            /// Enters next move from sequence of moves.
            ++current;
            /// Elapsed time is approximated to the hundredths of a second.
            time = approximate<double>(time, 0.01);
            time -= move.time;
            elapsed_time = time;
            applyNext();
            if(current >= moves.size())
            {
                /// There is no more moves in sequence of moves.
                if(en.pl->model.getCurrentState() != ANIMATION_STAND)
                    en.pl->model.setAnimationState(ANIMATION_STAND);
                en.processMove(0.0f);
                mode = Moves::MODE_NONE;
                break;
            }
        }
        else
            break;
    }
}
