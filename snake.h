#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QList>
#include "food.h"
#include "obstacle.h"

enum Direction { Up, Down, Left, Right };

class Snake : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Snake(QGraphicsScene *scene);
    void move();
    void grow();
    void spawnFood();
    void setDirection(Direction dir);
    Direction getCurrentDirection() const;
    bool isOppositeDirection(Direction newDirection);
    Direction nextDirection;
    bool canChangeDirection;

private slots:
    void spawnObstacle();

private:
    QList<Obstacle*> obstacles;
    QTimer* obstacleTimer;
    QTimer *moveTimer;
    QList<QGraphicsRectItem*> body;
    Direction currentDirection;
    QGraphicsScene *scene;
    Food* currentFood;
    int score;
    QGraphicsTextItem* scoreText;
    bool gameOver;
};

#endif // SNAKE_H
