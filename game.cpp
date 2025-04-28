#include "game.h"
#include <QKeyEvent>

Game::Game()
{
    scene = new QGraphicsScene(0, 0, 600, 400, this);
    setScene(scene);
    setFixedSize(620, 420);
    setSceneRect(0, 0, 600, 400);

    snake = new Snake(scene);
}

void Game::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if (snake->getCurrentDirection() != Down)
            snake->setDirection(Up);
        break;
    case Qt::Key_Down:
        if (snake->getCurrentDirection() != Up)
            snake->setDirection(Down);
        break;
    case Qt::Key_Left:
        if (snake->getCurrentDirection() != Right)
            snake->setDirection(Left);
        break;
    case Qt::Key_Right:
        if (snake->getCurrentDirection() != Left)
            snake->setDirection(Right);
        break;
    default:
        break;
    }
}
