#include "obstacle.h"
#include <QBrush>

Obstacle::Obstacle()
{
    setRect(0, 0, 20, 20);
    setBrush(Qt::red);
}
