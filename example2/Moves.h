/// Author: Przemysław Bągard

#ifndef MOVES_H
#define MOVES_H

#include <QVector>
#include <QTextStream>
#include "Geometry/Point.h"
#include "Player.h"

class GLView;

class Moves
{
public:
    Moves(GLView *view);

    /// Program mode.
    enum Mode {
        /// Controlling character.
        MODE_NONE,

        /// Playing sequence of moves.
        MODE_PLAY
    } mode;

    /// Movement type.
    enum STATE {
        /// stand
        STATE_STAND,

        /// move forward
        STATE_GO_FRONT,

        /// go back
        STATE_GO_BACK,

        /// rotate
        STATE_ROTATE
    };

    /// Movement data
    struct Move {
        /// Angle - relative to current movement
        float rad;

        /// Movement time.
        double time;

        /// Type of movement (move forward, go back, rotate, stand)
        STATE state;
    };

    /// Loads moves from file.
    bool loadFromFile(const QString& name);

    /// Adds move to sequence of moves.
    void addMove(const Move& move);

    /// Returns current movement index.
    int getCurrent() const;

    /// Backs to the first movement from the sequence.
    void firstMove();

    /// Enters new move from sequence if needed.
    void playAnimation(double elapsed_time);

    /// Enters next movement from the sequence.
    void applyNext();

    /// Error while loading sequence of moves.
    void loadError(int lineNumber);

    /// Returns the time from the start of the current move.
    float getEventCurrentTime();

    /// Returns the time of the current move.
    float getEventCurrentMaxTime();
private:
    /// Character position at the start.
    Point start;

    /// Sequence of moves
    QVector<Move> moves;

    /// Current move index from sequence.
    int current;

    /// Time from the start of the current move.
    double time;

    /// Character direction range at the start of rotation.
    float startRad;

    /// Pointer to openGL widget.
    GLView* view;
};

#endif // MOVES_H
