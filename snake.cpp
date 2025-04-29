#include "snake.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QApplication>

Snake::Snake(QGraphicsScene *scene) : scene(scene), currentFood(nullptr), gameOver()
{
    setRect(0, 0, 20, 20);
    setBrush(Qt::green);
    scene->addItem(this);

    body.append(this);

    currentDirection = Right;

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Snake::move);
    moveTimer->start(200);

    spawnFood();

    obstacleTimer = new QTimer(this);
    connect(obstacleTimer, &QTimer::timeout, this, &Snake::spawnObstacle);
    obstacleTimer->start(3000);

    score = 0;
    scoreText = new QGraphicsTextItem();
    scoreText->setPlainText("Очки: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 16));
    scoreText->setPos(10, 10);
    scene->addItem(scoreText);
}

void Snake::setDirection(Direction dir)
{
    if(!isOppositeDirection(dir)) currentDirection = dir;
}

void Snake::move()
{
    QPointF headPos = body.first()->pos();
    QPointF newPos = headPos;

    switch (currentDirection) {
    case Up:    newPos.setY(headPos.y() - 20); break;
    case Down:  newPos.setY(headPos.y() + 20); break;
    case Left:  newPos.setX(headPos.x() - 20); break;
    case Right: newPos.setX(headPos.x() + 20); break;
    }

    if (newPos.x() >= scene->width()) newPos.setX(0);
    if (newPos.x() < 0) newPos.setX(scene->width() - 20);
    if (newPos.y() >= scene->height()) newPos.setY(0);
    if (newPos.y() < 0) newPos.setY(scene->height() - 20);

    for (int i = body.size() - 1; i > 0; --i) {
        body[i]->setPos(body[i-1]->pos());
    }

    body.first()->setPos(newPos);

    for (int i = 1; i < body.size(); ++i) {
        if (body.first()->pos() == body[i]->pos()) {
            moveTimer->stop();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Игра окончена", "Вы проиграли! Начать заново?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                scene->clear();
                Snake* newSnake = new Snake(scene);
                scene->addItem(newSnake);
            } else {
                qApp->quit();
            }
            return;
        }
    }
    for (Obstacle* obs : obstacles) {
        if (body.first()->pos() == obs->pos()) {
            moveTimer->stop();
            obstacleTimer->stop();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Игра окончена", "Вы врезались в препятствие! Начать заново?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                scene->clear();
                Snake* newSnake = new Snake(scene);
                scene->addItem(newSnake);
            } else {
                qApp->quit();
            }
            return;
        }
    }

    if (currentFood) {
        QPointF foodPos = currentFood->pos();
        qreal dx = qAbs(body.first()->pos().x() - foodPos.x());
        qreal dy = qAbs(body.first()->pos().y() - foodPos.y());

        if ((dx <= 20 && dy == 0) || (dy <= 20 && dx == 0)) {
            scene->removeItem(currentFood);
            delete currentFood;
            currentFood = nullptr;
            grow();
            spawnFood();

            score += 10;
            scoreText->setPlainText("Очки: " + QString::number(score));
        }
    }
}

void Snake::grow()
{
    QGraphicsRectItem* newSegment = new QGraphicsRectItem(0, 0, 20, 20);
    newSegment->setBrush(QBrush(Qt::green));
    newSegment->setPos(body.last()->pos());
    scene->addItem(newSegment);
    body.append(newSegment);
}

void Snake::spawnFood()
{
    if (currentFood) {
        scene->removeItem(currentFood);
        delete currentFood;
    }

    currentFood = new Food();
    int x = (QRandomGenerator::global()->bounded(int(scene->width()) / 20)) * 20;
    int y = (QRandomGenerator::global()->bounded(int(scene->height()) / 20)) * 20;
    currentFood->setPos(x, y);
    scene->addItem(currentFood);
}

Direction Snake::getCurrentDirection() const
{
    return currentDirection;
}

void Snake::spawnObstacle()
{
    Obstacle* obs = new Obstacle();
    int x = (QRandomGenerator::global()->bounded(int(scene->width()) / 20)) * 20;
    int y = (QRandomGenerator::global()->bounded(int(scene->height()) / 20)) * 20;
    obs->setPos(x, y);
    scene->addItem(obs);
    obstacles.append(obs);

    QTimer::singleShot(10000, this, [this, obs]() {
        if (obstacles.contains(obs)) {
            scene->removeItem(obs);
            obstacles.removeOne(obs);
            delete obs;
        }
    });
}

bool Snake::isOppositeDirection(Direction newDirection) {
    switch (currentDirection) {
    case Up:
        return newDirection == Down;
    case Down:
        return newDirection == Up;
    case Left:
        return newDirection == Right;
    case Right:
        return newDirection == Left;
    default:
        return false;
    }
}
