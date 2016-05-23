#include <QPainter>
#include <QList>
#include <QtMath>
#include <QDebug>
#include <QGraphicsScene>

#include "ship.h"
#include "field.h"

Ship::Ship(int n, qreal x, Field *f) : n(n), x(x), field(f)
{
    orientation = HORIZONTAL;
    isDragged = false;
    isFixed = false;
    buttonIsPush = false;
    shipRect = QRect(0, 0, 0, 0);
}

QRectF Ship::boundingRect() const
{
    return QRectF(-n * x / 2, -n * x / 2, n * x, n * x);
}

void Ship::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(painter->Antialiasing);
    QPen p(Qt::blue);
    p.setWidth(3);
    painter->setPen(p);
    QBrush br(QColor(170, 170, 255, 100));
    painter->setBrush(br);
    if (orientation == VERTICAL)
    {
        painter->drawRect(QRectF(-x / 2, - n * x / 2, x, n * x));
    }
    else
    {
        painter->drawRect(QRectF(-n * x / 2, -x / 2, n * x, x));
    }
}

int Ship::getOrientation()
{
    return orientation;
}

int Ship::getN()
{
    return n;
}

qreal Ship::getX()
{
    return x;
}

void Ship::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isFixed)
        return;

    QRectF rct;
    if (orientation == HORIZONTAL)
        rct = QRectF(-n * x / 2, -x / 2, n * x, x);
    else
        rct = QRectF(-x / 2, -n * x / 2, x, n * x);
    if (!rct.contains(event->pos()))
        return;

    buttonIsPush = true;
    pPos = mapToScene(event->pos());
    beginPos = pos();
}

void Ship::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isFixed || !buttonIsPush)
        return;
    QPointF currPos = mapToScene(event->pos());
    QPointF v = currPos - pPos;
    if (isDragged)
    {
        QPointF shipPos = pos() + v;
        setPos(shipPos);
        pPos = currPos;
    }
    else
    {
        if (v.manhattanLength() > 4)
        {
            isDragged = true;
            QPointF shipPos = pos() + v;
            setPos(shipPos);
            pPos = currPos;
        }
    }
}

void Ship::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (isFixed || !buttonIsPush)
        return;
    QPointF currPos = mapToScene(event->pos());
    QPointF v = currPos - pPos;
    if (isDragged)
    {
        QPointF shipPos = pos() + v;
        setPos(shipPos);
    }
    else
    {
        if (v.manhattanLength() > 4)
        {
            isDragged = true;
            QPointF shipPos = pos() + v;
            setPos(shipPos);
        }
    }

    if (shipRect.width() != 0)
        field->unsetShip(shipRect.x(), shipRect.y(), shipRect.width(), shipRect.height());

    if (isDragged)
    {
        QRect rct;
        rct = field->setShip(this);
        if (rct.width() == 0)
        {
            setPos(beginPos);
            field->setShip(shipRect.x(), shipRect.y(), shipRect.width(), shipRect.height());
        }
        else
        {
            shipRect = rct;
        }

        isDragged = false;
    }
    else if (shipRect.width() != 0)
    {
        if (field->changeOrientation(shipRect.x(), shipRect.y(), shipRect.width(), shipRect.height()))
        {
            changeOrientation();
        }
    }
    isDragged = false;
    buttonIsPush = false;
}

void Ship::changeOrientation()
{
    int w = shipRect.width();
    int h = shipRect.height();
    shipRect.setWidth(h);
    shipRect.setHeight(w);
    if (orientation == HORIZONTAL)
    {
        orientation = VERTICAL;
        QPointF p = pos();
        setPos(p.x() - n * x / 2 + x / 2, p.y() + n * x / 2 - x / 2);
    }
    else
    {
        orientation = HORIZONTAL;
        QPointF p = pos();
        setPos(p.x() + n * x / 2 - x / 2, p.y() - n * x / 2 + x / 2);
    }
    update(boundingRect());
}

bool Ship::onField()
{
    return (shipRect.width() != 0);
}

void Ship::setFixed()
{
    isFixed = true;
}

void Ship::setDamage()
{
    isDamage = true;
}

void Ship::setRect(QRect r)
{
    shipRect = r;
}
