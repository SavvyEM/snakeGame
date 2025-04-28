#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "snake.h"

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game();
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    QGraphicsScene *scene;
    Snake *snake;
};

#endif // GAME_H
