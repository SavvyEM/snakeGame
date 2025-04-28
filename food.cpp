#include "food.h"
#include <QBrush>

Food::Food()
{
    setRect(0, 0, 20, 20);
    setBrush(QBrush(Qt::yellow));
}
