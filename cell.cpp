#include "cell.h"
#include <QDebug>

Cell::Cell(qreal _size, int row, int column) : _size(_size), row(row), column(column)
{
    isPoint = false;
    isCross = false;
}

QRectF Cell::boundingRect() const
{
    return QRectF(-_size / 2, -_size / 2, _size, _size);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(painter->Antialiasing);
    if (isPoint)
    {
        QBrush br(Qt::blue);
        QPen p(Qt::blue);
        p.setWidth(6);
        painter->setBrush(br);
        painter->setPen(p);
        painter->drawPoint(0, 0);
    }
    if (isCross)
    {
        QPen p(Qt::green);
        QBrush br(Qt::green);
        p.setWidth(2);
        painter->setPen(p);
        painter->setBrush(br);
        painter->drawLine(boundingRect().topLeft(), boundingRect().bottomRight());
        painter->drawLine(boundingRect().topRight(), boundingRect().bottomLeft());
    }
}

void Cell::setIsCross()
{
    isCross = true;
    update(boundingRect());
}

void Cell::setIsPoint()
{
    isPoint = true;
    update(boundingRect());
}

/*void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "PUSH";
    emit clicked(column, row);
}*/
