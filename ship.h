#ifndef SHIP_H
#define SHIP_H

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class Field;

class Ship : public QGraphicsItem
{
public:
    Ship(int n, qreal x, Field *f);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void changeOrientation();
    int getN();
    qreal getX();

    enum Orientation {VERTICAL, HORIZONTAL};
    int getOrientation();
    bool onField();
    void setFixed();
    void setDamage();
    void setRect(QRect r);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
    Orientation orientation;
    int n;
    qreal x;
    QPointF pPos;
    QPointF beginPos;
    bool isDragged;
    QRect shipRect;
    bool isFixed;
    bool isDamage;
    bool buttonIsPush;

    Field *field;
};

#endif // SHIP_H
